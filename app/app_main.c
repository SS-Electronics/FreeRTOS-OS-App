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
        // ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // count++;
        // gpio_mgmt_post(LED_STATUS, GPIO_MGMT_CMD_TOGGLE, 0, 0);
        printk("[BTN] Button pressed — count %lu\n\r", (unsigned long)count);
        os_thread_delay(1000);
    }
}

/* ── Application entry point ─────────────────────────────────────────────── */

int app_main(void)
{
    os_thread_create(heartbeat_task, "heartbeat",
                     APP_HEARTBEAT_STACK, APP_HEARTBEAT_PRIO, NULL);

    os_thread_create(btn_task, "btn",
                     APP_BTN_STACK, APP_BTN_PRIO, NULL);


    return 0;
}
