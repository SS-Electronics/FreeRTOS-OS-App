/*
 * app_main.c — Application entry point for FreeRTOS-OS
 *
 * This file is part of FreeRTOS-OS Project.
 *
 * IRQ chain (full Linux-style irq_desc path)
 * ──────────────────────────────────────────
 *
 *  [Hardware interrupt fires]
 *         │
 *         ▼  CPU vector table → irq_periph_dispatch_generated.c ISR
 *  drv_irq_dispatch_from_isr(IRQ_ID_xxx, &data, &hpt)
 *         │
 *         ▼  __do_IRQ_from_isr()
 *  irq_to_desc(irq_id)  →  desc->handle_irq()  →  handle_irq_event()
 *         │
 *         ▼  walk irqaction chain
 *  irq_handler_t (registered via request_irq)
 *         │
 *         ▼  vTaskNotifyGiveFromISR / ringbuffer / etc.
 *  FreeRTOS task unblocks
 *
 * Tasks
 * ─────
 *   heartbeat_task — toggles LED_BOARD every 500 ms
 *   btn_task       — toggles LED_STATUS on each BTN_USER press via IRQ_ID_EXTI(0)
 *                    logs to UART_DEBUG via printk
 *   uart_rx_task   — receives bytes from USART1 (UART_DEBUG) via IRQ callback
 *                    irq_register(IRQ_ID_UART_RX(UART_DEBUG)) → ISR puts byte in
 *                    private ring buffer → task wakes and processes
 *   os_shell_mgmt  — interactive CLI over UART_APP (USART2, /dev/ttyUSB0)
 *                    connect with PuTTY: 115200 8N1, no flow control
 */

#include <os/kernel.h>
#include <os/kernel_syscall.h>
#include <services/gpio_mgmt.h>
#include <services/os_shell_management.h>
#include <board/board_device_ids.h>
#include <irq/irq_desc.h>

/* ── Task configuration ──────────────────────────────────────────────────── */

#define APP_HEARTBEAT_STACK   256
#define APP_HEARTBEAT_PRIO    1
#define APP_HEARTBEAT_PERIOD  500   /* ms */

#define APP_BTN_STACK         256
#define APP_BTN_PRIO          2

/* BTN_USER is wired to PA0 → EXTI line 0 (see app/board/stm32f411_devboard.xml) */
#define BTN_USER_PIN          0

#define APP_UART_RX_STACK     256
#define APP_UART_RX_PRIO      1
#define APP_UART_RX_BUF_LEN   64   /* must be power-of-2 for safe wrapping */

/* ── UART_DEBUG (USART1) RX — IRQ callback + task ───────────────────────── */

/*
 * Private ring buffer written by _uart_debug_rx_cb() in ISR context and
 * drained by uart_rx_task() in task context.  head is volatile so the
 * compiler does not cache it across the while(_rx_tail != _rx_head) loop.
 */
static uint8_t           _rx_buf[APP_UART_RX_BUF_LEN];
static volatile uint16_t _rx_head = 0;   /* ISR writes */
static uint16_t          _rx_tail = 0;   /* task reads  */

static TaskHandle_t _uart_rx_task_handle = NULL;

/*
 * _uart_debug_rx_cb — irq_notify callback, runs in ISR context.
 *
 * data : pointer to the received byte (set by hal_uart_stm32.c before calling
 *        drv_irq_dispatch_from_isr(IRQ_ID_UART_RX(n), &byte, &hpt)).
 */
static void _uart_debug_rx_cb(irq_id_t id, void *data,
                               void *arg, BaseType_t *pxHPT)
{
    (void)id;
    (void)arg;

    if (data == NULL)
        return;

    uint16_t next = (_rx_head + 1) % APP_UART_RX_BUF_LEN;
    if (next != _rx_tail) {          /* silently drop if buffer full */
        _rx_buf[_rx_head] = *(uint8_t *)data;
        _rx_head = next;
    }

    if (_uart_rx_task_handle != NULL)
        vTaskNotifyGiveFromISR(_uart_rx_task_handle, pxHPT);
}

static void uart_rx_task(void *param)
{
    (void)param;
    _uart_rx_task_handle = xTaskGetCurrentTaskHandle();

    /*
     * Subscribe to UART_APP (USART2) RX events — separate from the shell UART.
     * irq_register() chains this alongside uart_mgmt's ringbuffer callback;
     * both fire per received byte without conflict.
     */
    irq_register(IRQ_ID_UART_RX(UART_APP), _uart_debug_rx_cb, NULL);

    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        while (_rx_tail != _rx_head)
        {
            uint8_t byte = _rx_buf[_rx_tail];
            _rx_tail = (_rx_tail + 1) % APP_UART_RX_BUF_LEN;

            printk("[UART_APP RX] 0x%02X '%c'\r\n",
                   byte,
                   (byte >= 0x20U && byte < 0x7FU) ? (char)byte : '.');
        }
    }
}

/* ── Heartbeat task ──────────────────────────────────────────────────────── */

static void heartbeat_task(void *param)
{
    (void)param;
    while (1)
    {
        gpio_mgmt_post(LED_BOARD, GPIO_MGMT_CMD_TOGGLE, 0, 0);
        os_thread_delay(APP_HEARTBEAT_PERIOD);
    }
}

/* ── Button task — EXTI interrupt via Linux-style request_irq ────────────── */

/*
 * BTN_USER is wired to PA0 → EXTI line 0.
 * irq_periph_dispatch_generated.c:EXTI0_IRQHandler() clears the pending bit and calls
 * drv_irq_dispatch_from_isr(IRQ_ID_EXTI(0), &pin, &hpt).
 *
 * That enters the irq_desc chain:
 *   __do_IRQ_from_isr → irq_to_desc(IRQ_ID_EXTI(0))
 *     → handle_simple_irq → handle_irq_event
 *       → _btn_irq_handler  (registered below via request_irq)
 *
 * Button events are logged via printk() which goes to UART_DEBUG (USART1),
 * keeping UART_APP (USART2) clean for the interactive shell.
 */

static TaskHandle_t _btn_task_handle = NULL;

static irqreturn_t _btn_irq_handler(irq_id_t   irq,
                                    void       *data,
                                    void       *dev_id,
                                    BaseType_t *pxHPT)
{
    (void)irq;
    (void)data;
    TaskHandle_t task = (TaskHandle_t)dev_id;
    if (task != NULL)
        vTaskNotifyGiveFromISR(task, pxHPT);
    return IRQ_HANDLED;
}

static void btn_task(void *param)
{
    (void)param;
    _btn_task_handle = xTaskGetCurrentTaskHandle();

    /*
     * request_irq — Linux-style direct registration into the irq_desc chain.
     * dev_id is the task handle; _btn_irq_handler casts it back and wakes us.
     */
    request_irq(IRQ_ID_EXTI(BTN_USER_PIN), _btn_irq_handler,
                "btn_user", _btn_task_handle);

    uint32_t count = 0;

    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        count++;
        gpio_mgmt_post(LED_STATUS, GPIO_MGMT_CMD_TOGGLE, 0, 0);
        printk("[BTN] Button pressed — count %lu\r\n", (unsigned long)count);
    }
}

/* ── Application entry point ─────────────────────────────────────────────── */

int app_main(void)
{
    os_thread_create(heartbeat_task, "heartbeat",
                     APP_HEARTBEAT_STACK, APP_HEARTBEAT_PRIO, NULL);

    os_thread_create(btn_task, "btn",
                     APP_BTN_STACK, APP_BTN_PRIO, NULL);

    os_thread_create(uart_rx_task, "uart_rx",
                     APP_UART_RX_STACK, APP_UART_RX_PRIO, NULL);

    return 0;
}
