/*
 * AUTO-GENERATED — DO NOT EDIT
 * Generator : scripts/gen_board_config.py
 * Source    : ../app/board/stm32h723_devboard.xml
 * Date      : 2026-06-24
 *
 * Re-generate:
 *   python3 scripts/gen_board_config.py ../app/board/stm32h723_devboard.xml
 */

#ifndef BOARD_STM32H723_DEMO_DEVBOARD_HANDLES_H_
#define BOARD_STM32H723_DEMO_DEVBOARD_HANDLES_H_

/* Board: STM32H723 Demo Devboard  MCU: STM32H723ZGTx */

#ifdef HAL_UART_MODULE_ENABLED
#include "stm32h7xx_hal_uart.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
#endif /* HAL_UART_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
#include "stm32h7xx_hal_i2c.h"
extern I2C_HandleTypeDef hi2c1;
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
#include "stm32h7xx_hal_spi.h"
extern SPI_HandleTypeDef hspi1;
#endif /* HAL_SPI_MODULE_ENABLED */

#endif /* BOARD_STM32H723_DEMO_DEVBOARD_HANDLES_H_ */
