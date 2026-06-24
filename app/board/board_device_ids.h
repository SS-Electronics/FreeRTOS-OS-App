/*
 * AUTO-GENERATED — DO NOT EDIT
 * Generator : scripts/gen_board_config.py
 * Source    : ../app/board/stm32h723_devboard.xml
 * Date      : 2026-06-24
 *
 * Re-generate:
 *   python3 scripts/gen_board_config.py ../app/board/stm32h723_devboard.xml
 */

#ifndef BOARD_STM32H723_DEMO_DEVBOARD_DEVICE_IDS_H_
#define BOARD_STM32H723_DEMO_DEVBOARD_DEVICE_IDS_H_

#include "autoconf.h"

/* Board: STM32H723 Demo Devboard  MCU: STM32H723ZGTx */

/* UART device IDs */
#define UART_APP                         0
#define UART_DEBUG                       1
#define BOARD_UART_COUNT             2

/* I2C device IDs */
#define I2C_SENSOR_BUS                   0
#define BOARD_IIC_COUNT             1

/* SPI device IDs */
#define SPI_FLASH_BUS                    0
#define BOARD_SPI_COUNT             1

/* GPIO device IDs */
#define LED_BOARD                        0
#define LED_YELLOW                       1
#define BTN_USER                         2
#define BOARD_GPIO_COUNT             3

/* UART role assignments */
#define BOARD_UART_SHELL_ID          UART_DEBUG           /* role="shell" */
#define UART_SHELL_HW_ID             BOARD_UART_SHELL_ID  /* printk / shell UART */

#endif /* BOARD_STM32H723_DEMO_DEVBOARD_DEVICE_IDS_H_ */
