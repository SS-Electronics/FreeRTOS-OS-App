/*
 * app_main.c — Application entry point for FreeRTOS-OS
 *
 * Three tasks demonstrate the system:
 *   1. heartbeat_task  — toggles LED_BOARD every 500 ms.
 *   2. hello_task      — sends a fixed string over UART_DEBUG every 2 s.
 *   3. echo_task       — reads bytes from UART_DEBUG RX and echoes them back.
 *
 * app_main() is called by init/main.c before vTaskStartScheduler().
 * It creates the tasks and returns; the scheduler drives them.
 */

#include <os/kernel.h>
#include <services/gpio_mgmt.h>
#include <services/uart_mgmt.h>
#include <board/board_device_ids.h>

/* ── Task configuration ─────────────────────────────────────────────────── */

#define APP_HEARTBEAT_STACK   256
#define APP_HEARTBEAT_PRIO    2
#define APP_HEARTBEAT_PERIOD  500   /* ms */

#define APP_HELLO_STACK       512
#define APP_HELLO_PRIO        1
#define APP_HELLO_INTERVAL    2000  /* ms */

#define APP_ECHO_STACK        512
#define APP_ECHO_PRIO         1


/* ── Heartbeat task — toggles LED_BOARD every 500 ms ───────────────────── */

static void heartbeat_task(void *param)
{
    (void)param;

    while (1)
    {
        gpio_mgmt_post(LED_BOARD, GPIO_MGMT_CMD_TOGGLE, 0, 0);
        os_thread_delay(APP_HEARTBEAT_PERIOD);
    }
}


/* ── Hello task — periodic UART TX ─────────────────────────────────────── */

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


/* ── Echo task — read RX bytes and send them back ──────────────────────── */
/*
 * Uses a 256-byte rotating pool so the pointer passed to
 * uart_mgmt_async_transmit() stays valid until the management thread
 * drains its queue (at most 256 in-flight bytes before wrap-around).
 */
static void echo_task(void *param)
{
    (void)param;

    static uint8_t tx_pool[256];
    uint8_t        pool_idx = 0;
    uint8_t        rx_byte;

    while (1)
    {
        if (uart_mgmt_read_byte(UART_DEBUG, &rx_byte) == OS_ERR_NONE)
        {
            tx_pool[pool_idx] = rx_byte;
            uart_mgmt_async_transmit(UART_DEBUG, &tx_pool[pool_idx], 1U);
            pool_idx = (uint8_t)(pool_idx + 1U);
        }
        else
        {
            os_thread_delay(1);
        }
    }
}


/* ── Application entry point ────────────────────────────────────────────── */

int app_main(void)
{
    os_thread_create(heartbeat_task, "heartbeat",
                     APP_HEARTBEAT_STACK, APP_HEARTBEAT_PRIO, NULL);

    os_thread_create(hello_task, "hello",
                     APP_HELLO_STACK, APP_HELLO_PRIO, NULL);

    os_thread_create(echo_task, "echo",
                     APP_ECHO_STACK, APP_ECHO_PRIO, NULL);

    return 0;
}
