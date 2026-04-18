/*
 * app_main.c — Example application for FreeRTOS-OS
 *
 * Demonstrates the two most common patterns:
 *   1. A periodic LED heartbeat task using gpio_mgmt.
 *   2. A UART hello-world task using uart_mgmt.
 *
 * app_main() is called by init/main.c after the OS scheduler starts.
 * It creates application tasks and returns; the scheduler drives them.
 *
 * Board assumptions (stm32f411_devboard):
 *   LED_BOARD  — board device ID 0  (PC13, active-low)
 *   UART_DEBUG — UART device ID 0   (USART1, 115200 baud)
 */

#include <os/kernel.h>
#include <services/gpio_mgmt.h>
#include <services/uart_mgmt.h>

/* ── Task configuration ─────────────────────────────────────────────── */

#define APP_HEARTBEAT_STACK   256
#define APP_HEARTBEAT_PRIO    2
#define APP_HEARTBEAT_LED_ID  0     /* LED_BOARD device id from board_device_ids.h */
#define APP_HEARTBEAT_PERIOD  500   /* ms — toggle every 500 ms */

#define APP_HELLO_STACK       512
#define APP_HELLO_PRIO        1
#define APP_HELLO_UART_ID     0     /* UART_DEBUG device id */
#define APP_HELLO_INTERVAL    2000  /* ms — print every 2 s */

/* ── Tasks ──────────────────────────────────────────────────────────── */

static void heartbeat_task(void *param)
{
    (void)param;

    while (1)
    {
        gpio_mgmt_post(APP_HEARTBEAT_LED_ID, GPIO_MGMT_CMD_TOGGLE, 0, 0);
        os_thread_delay(APP_HEARTBEAT_PERIOD);
    }
}

static void hello_task(void *param)
{
    (void)param;

    static const uint8_t msg[] = "[APP] Hello from FreeRTOS-OS application!\r\n";

    while (1)
    {
        uart_mgmt_async_transmit(APP_HELLO_UART_ID, msg, sizeof(msg) - 1);
        os_thread_delay(APP_HELLO_INTERVAL);
    }
}

/* ── Entry point ────────────────────────────────────────────────────── */

int app_main(void)
{
    os_thread_create(heartbeat_task, "heartbeat",
                     APP_HEARTBEAT_STACK, APP_HEARTBEAT_PRIO, NULL);

    os_thread_create(hello_task, "hello",
                     APP_HELLO_STACK, APP_HELLO_PRIO, NULL);

    return 0;
}
