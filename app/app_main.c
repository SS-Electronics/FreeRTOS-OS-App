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
 *         ▼  CPU vector table → hal_it_stm32.c ISR
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
 *   hello_task     — sends a banner over UART_DEBUG every 2 s
 *   echo_task      — event-driven UART echo via IRQ_ID_UART_RX
 *   btn_task       — toggles LED_STATUS on each BTN_USER press via IRQ_ID_EXTI(0)
 */

#include <os/kernel.h>
#include <services/gpio_mgmt.h>
#include <services/uart_mgmt.h>
#include <board/board_device_ids.h>
#include <irq/irq_notify.h>
#include <irq/irq_desc.h>

/* ── Task configuration ──────────────────────────────────────────────────── */

#define APP_HEARTBEAT_STACK   256
#define APP_HEARTBEAT_PRIO    1
#define APP_HEARTBEAT_PERIOD  500   /* ms */

#define APP_HELLO_STACK       512
#define APP_HELLO_PRIO        1
#define APP_HELLO_INTERVAL    2000  /* ms */

#define APP_ECHO_STACK        512
#define APP_ECHO_PRIO         2

#define APP_BTN_STACK         256
#define APP_BTN_PRIO          3     /* higher than echo — button is latency-sensitive */

/* BTN_USER is wired to PA0 → EXTI line 0 (see app/board/stm32f411_devboard.xml) */
#define BTN_USER_PIN          0

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

/* ── Hello task ──────────────────────────────────────────────────────────── */

static void hello_task(void *param)
{
    (void)param;
    static const uint8_t msg[] = "[APP] Hello from FreeRTOS-OS!\r\n";
    while (1)
    {
        uart_mgmt_async_transmit(UART_DEBUG, msg, sizeof(msg) - 1U);
        os_thread_delay(APP_HELLO_INTERVAL);
    }
}

/* ── Echo task — event-driven UART RX via irq_notify ─────────────────────── */

static TaskHandle_t _echo_task_handle = NULL;

/*
 * _echo_uart_rx_cb — irq_notify_cb_t subscriber for IRQ_ID_UART_RX(UART_DEBUG).
 *
 * Registered via irq_register() which wraps it in an irqaction trampoline and
 * chains it into the irq_desc for IRQ_ID_UART_RX(UART_DEBUG).
 * Called from handle_irq_event() inside the irq_desc dispatch chain, ISR context.
 */
static void _echo_uart_rx_cb(irq_id_t    id,
                              void       *data,
                              void       *arg,
                              BaseType_t *pxHPT)
{
    (void)id;
    (void)data;
    (void)arg;
    if (_echo_task_handle != NULL)
        vTaskNotifyGiveFromISR(_echo_task_handle, pxHPT);
}

static void echo_task(void *param)
{
    (void)param;
    _echo_task_handle = xTaskGetCurrentTaskHandle();

    /*
     * irq_register() → allocates a trampoline → request_irq() → appends
     * an irqaction to IRQ_ID_UART_RX(UART_DEBUG)'s descriptor chain.
     * Multiple subscribers allowed (up to IRQ_NOTIFY_MAX_SUBS).
     */
    irq_register(IRQ_ID_UART_RX(UART_DEBUG), _echo_uart_rx_cb, NULL);

    static uint8_t tx_pool[256];
    uint8_t        pool_idx = 0;

    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        uint8_t rx_byte;
        while (uart_mgmt_read_byte(UART_DEBUG, &rx_byte) == OS_ERR_NONE)
        {
            tx_pool[pool_idx] = rx_byte;
            uart_mgmt_async_transmit(UART_DEBUG, &tx_pool[pool_idx], 1U);
            pool_idx = (uint8_t)(pool_idx + 1U);
        }
    }
}

/* ── Button task — EXTI interrupt via Linux-style request_irq ────────────── */

/*
 * BTN_USER is wired to PA0 → EXTI line 0.
 * hal_it_stm32.c:EXTI0_IRQHandler() clears the pending bit and calls
 * drv_irq_dispatch_from_isr(IRQ_ID_EXTI(0), &pin, &hpt).
 *
 * That enters the irq_desc chain:
 *   __do_IRQ_from_isr → irq_to_desc(IRQ_ID_EXTI(0))
 *     → handle_simple_irq → handle_irq_event
 *       → _btn_irq_handler  (registered below via request_irq)
 *
 * _btn_irq_handler uses the irq_handler_t signature (returns irqreturn_t)
 * which is the native format for irqaction.  No trampoline needed.
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
     *
     * The NVIC line (EXTI0_IRQn) and its priority were bound by
     * irq_hw_init_all() (generated from irq_table.xml) at boot.
     */
    request_irq(IRQ_ID_EXTI(BTN_USER_PIN), _btn_irq_handler,
                "btn_user", _btn_task_handle);

    static const uint8_t btn_msg[] = "[BTN] Button pressed!\r\n";

    while (1)
    {
        /* Sleep until the EXTI ISR fires _btn_irq_handler */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        /* Toggle the status LED and log */
        gpio_mgmt_post(LED_STATUS, GPIO_MGMT_CMD_TOGGLE, 0, 0);
        uart_mgmt_async_transmit(UART_DEBUG, btn_msg, sizeof(btn_msg) - 1U);
    }
}

/* ── Application entry point ─────────────────────────────────────────────── */

int app_main(void)
{
    os_thread_create(heartbeat_task, "heartbeat",
                     APP_HEARTBEAT_STACK, APP_HEARTBEAT_PRIO, NULL);

    os_thread_create(hello_task, "hello",
                     APP_HELLO_STACK, APP_HELLO_PRIO, NULL);

    os_thread_create(echo_task, "echo",
                     APP_ECHO_STACK, APP_ECHO_PRIO, NULL);

    os_thread_create(btn_task, "btn",
                     APP_BTN_STACK, APP_BTN_PRIO, NULL);

    return 0;
}
