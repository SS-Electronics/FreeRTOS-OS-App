/*
 * AUTO-GENERATED — DO NOT EDIT
 * Generator : scripts/gen_board_config.py
 * Source    : app/board/stm32f411_devboard.xml
 * Date      : 2026-04-20
 *
 * Re-generate:
 *   python3 scripts/gen_board_config.py app/board/stm32f411_devboard.xml
 */

#ifndef BOARD_MCU_CONFIG_H_
#define BOARD_MCU_CONFIG_H_

/* Board: stm32f411_devboard  MCU: STM32F411VET6 */

#include "autoconf.h"

/* ── MCU Vendor / Variant Selection ──────────────────────────────────────── */
#define MCU_VAR_MICROCHIP   1
#define MCU_VAR_STM         2

#define CONFIG_DEVICE_VARIANT    MCU_VAR_STM   /* STM32F411VET6 */

/* ── Hardware-fixed peripheral instance counts ───────────────────────────── */
#define MCU_MAX_UART_PERIPHERAL     3
#define MCU_MAX_IIC_PERIPHERAL      3
#define MCU_MAX_SPI_PERIPHERAL      5
#define MCU_MAX_TIM_PERIPHERAL      8
/* No CAN, No DAC, No FMC, No ETH on STM32F411 */

/* ── Board device IDs — generated from XML by gen_board_config.py ─────────── */
/* Defines BOARD_UART_COUNT, BOARD_IIC_COUNT, BOARD_SPI_COUNT, BOARD_GPIO_COUNT */
#include <board/board_device_ids.h>

/* ── OS-managed peripheral counts — sourced from board config ────────────── */
#ifdef BOARD_UART_COUNT
  #define NO_OF_UART    BOARD_UART_COUNT
#else
  #define NO_OF_UART    1   /* fallback — run 'make board-gen' */
#endif

#ifdef BOARD_IIC_COUNT
  #define NO_OF_IIC     BOARD_IIC_COUNT
#else
  #define NO_OF_IIC     0   /* fallback — run 'make board-gen' */
#endif

#ifdef BOARD_SPI_COUNT
  #define NO_OF_SPI     BOARD_SPI_COUNT
#else
  #define NO_OF_SPI     0   /* fallback — run 'make board-gen' */
#endif

#ifdef BOARD_GPIO_COUNT
  #define NO_OF_GPIO    BOARD_GPIO_COUNT
#else
  #define NO_OF_GPIO    0   /* fallback — run 'make board-gen' */
#endif

/* ── Peripheral enable flags ─────────────────────────────────────────────── */
#ifdef CONFIG_HAL_IWDG_MODULE_ENABLED
  #define CONFIG_MCU_WDG_EN               (1)
#else
  #define CONFIG_MCU_WDG_EN               (0)
#endif

#define CONFIG_MCU_FLASH_DRV_EN           (1)   /* Always available */

#define CONFIG_MCU_NO_OF_UART_PERIPHERAL  NO_OF_UART
#define CONFIG_MCU_NO_OF_IIC_PERIPHERAL   NO_OF_IIC
#define CONFIG_MCU_NO_OF_SPI_PERIPHERAL   NO_OF_SPI
#define CONFIG_MCU_NO_OF_GPIO_PERIPHERAL  NO_OF_GPIO

#define CONFIG_MCU_NO_OF_RS485_PERIPHERAL (0)
#define CONFIG_MCU_NO_OF_CAN_PERIPHERAL   (0)
#define CONFIG_MCU_NO_OF_USB_PERIPHERAL   (0)
#define CONFIG_MCU_NO_OF_ETH_PERIPHERAL   (0)

#ifdef CONFIG_HAL_TIM_MODULE_ENABLED
  #define CONFIG_MCU_NO_OF_TIMER_PERIPHERAL (2)
#else
  #define CONFIG_MCU_NO_OF_TIMER_PERIPHERAL (0)
#endif

#define NO_OF_TIMER   CONFIG_MCU_NO_OF_TIMER_PERIPHERAL

/* ── Individual UART instance enable — derived from board UART count ─────── */
#define UART_1_EN    (NO_OF_UART >= 1 ? 1 : 0)
#define UART_2_EN    (NO_OF_UART >= 2 ? 1 : 0)
#define UART_3_EN    (0)
#define UART_4_EN    (0)
#define UART_5_EN    (0)
#define UART_6_EN    (NO_OF_UART >= 3 ? 1 : 0)
#define UART_7_EN    (0)
#define UART_8_EN    (0)

/* ── Peripheral identifier enumerations ─────────────────────────────────── */
typedef enum
{
    UART_1 = 0,  /* USART1 */
    UART_2,  /* USART2 */
    UART_3,  /* not on F411, placeholder */
    UART_4,  /* not on F411, placeholder */
    UART_5,  /* not on F411, placeholder */
    UART_6,  /* USART6 */
    UART_7,
    UART_8
} UART_PORTS;

typedef enum
{
    IIC_1 = 0,
    IIC_2,
    IIC_3
} IIC_PORTS;

typedef enum
{
    TIMER_1,
    TIMER_2,
    TIMER_3,
    TIMER_4,
    INVALID_TIMER_ID
} TIMER_PORTS;

#endif /* BOARD_MCU_CONFIG_H_ */
