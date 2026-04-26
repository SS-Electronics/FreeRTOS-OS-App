/*
 * AUTO-GENERATED — DO NOT EDIT
 * Generator : scripts/gen_board_config.py
 * Source    : ../app/board/stm32f411_devboard.xml
 * Date      : 2026-04-26
 *
 * Re-generate:
 *   python3 scripts/gen_board_config.py ../app/board/stm32f411_devboard.xml
 */

#ifndef INCLUDE_BOARD_BOARD_CONFIG_H_
#define INCLUDE_BOARD_BOARD_CONFIG_H_

/* Board: stm32f411_devboard  MCU: STM32F411VET6 */

#include "autoconf.h"

/* ── MCU Vendor / Variant Selection ──────────────────────────────────────── */
#define MCU_VAR_MICROCHIP   1
#define MCU_VAR_STM         2
#define MCU_VAR_INFINEON    3

#define CONFIG_DEVICE_VARIANT    MCU_VAR_STM   /* STM32F411VET6 */

/* ── Board device IDs — generated from XML by gen_board_config.py ─────────── */
/* Defines BOARD_UART_COUNT, BOARD_IIC_COUNT, BOARD_SPI_COUNT, BOARD_GPIO_COUNT */
#include <board/board_device_ids.h>

#ifdef CONFIG_HAL_TIM_MODULE_ENABLED
  #define NO_OF_TIMER  (2)
#else
  #define NO_OF_TIMER  (0)
#endif

#ifdef CONFIG_HAL_IWDG_MODULE_ENABLED
  #define CONFIG_MCU_WDG_EN  (1)
#else
  #define CONFIG_MCU_WDG_EN  (0)
#endif

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

/* ══════════════════════════════════════════════════════════════════════════
 * Board peripheral configuration — types and API
 * ══════════════════════════════════════════════════════════════════════════ */

#include <def_std.h>
#include <device.h>     /* CMSIS types: GPIO_TypeDef, USART_TypeDef, IRQn_Type … */

#ifdef __cplusplus
extern "C" {
#endif

/* ── Clock tree  (STM32F411VET6 @ 100 MHz, HSI source) ──── */
#define BOARD_RCC_PLLM          16U
#define BOARD_RCC_PLLN          200U
#define BOARD_RCC_PLLP          RCC_PLLP_DIV2
#define BOARD_RCC_PLLQ          4U

#define BOARD_SYSCLK_HZ         100000000UL
#define BOARD_APB1_HZ           50000000UL
#define BOARD_APB2_HZ           100000000UL

#define BOARD_FLASH_LATENCY     FLASH_LATENCY_3

extern          uint32_t SystemCoreClock; /* = BOARD_SYSCLK_HZ at boot — updated by SystemCoreClockUpdate() */
extern const uint8_t     AHBPrescTable[16];
extern const uint8_t     APBPrescTable[8];

typedef struct {
    GPIO_TypeDef *port;       /**< GPIOA … GPIOE                              */
    uint16_t      pin;        /**< GPIO_PIN_0 … GPIO_PIN_15                   */
    uint32_t      mode;       /**< GPIO_MODE_AF_PP, GPIO_MODE_OUTPUT_PP, …    */
    uint32_t      pull;       /**< GPIO_NOPULL / GPIO_PULLUP / GPIO_PULLDOWN  */
    uint32_t      speed;      /**< GPIO_SPEED_FREQ_LOW / _MEDIUM / _HIGH …   */
    uint32_t      alternate;  /**< GPIO_AF7_USART1, GPIO_AF4_I2C1, …         */
} board_pin_t;

typedef struct {
    uint8_t        dev_id;
    USART_TypeDef *instance;
    uint32_t       baudrate;
    uint32_t       word_len;    /**< UART_WORDLENGTH_8B / _9B           */
    uint32_t       stop_bits;   /**< UART_STOPBITS_1 / _2               */
    uint32_t       parity;      /**< UART_PARITY_NONE / _EVEN / _ODD    */
    uint32_t       mode;        /**< UART_MODE_TX_RX / _TX / _RX        */
    board_pin_t    tx_pin;
    board_pin_t    rx_pin;
    IRQn_Type      irqn;
    uint32_t       irq_priority;
} board_uart_desc_t;

typedef struct {
    uint8_t      dev_id;
    I2C_TypeDef *instance;
    uint32_t     clock_hz;     /**< 100000 (standard) or 400000 (fast) */
    uint32_t     addr_mode;    /**< I2C_ADDRESSINGMODE_7BIT / _10BIT   */
    uint32_t     dual_addr;    /**< I2C_DUALADDRESS_DISABLE / _ENABLE  */
    board_pin_t  scl_pin;
    board_pin_t  sda_pin;
    IRQn_Type    ev_irqn;
    IRQn_Type    er_irqn;
    uint32_t     irq_priority;
} board_iic_desc_t;

typedef struct {
    uint8_t      dev_id;
    SPI_TypeDef *instance;
    uint32_t     mode;         /**< SPI_MODE_MASTER / _SLAVE           */
    uint32_t     direction;    /**< SPI_DIRECTION_2LINES / _1LINE      */
    uint32_t     data_size;    /**< SPI_DATASIZE_8BIT / _16BIT         */
    uint32_t     clk_polarity; /**< SPI_POLARITY_LOW / _HIGH           */
    uint32_t     clk_phase;    /**< SPI_PHASE_1EDGE / _2EDGE           */
    uint32_t     nss;          /**< SPI_NSS_SOFT / _HARD_OUTPUT        */
    uint32_t     baud_prescaler;
    uint32_t     bit_order;    /**< SPI_FIRSTBIT_MSB / _LSB            */
    board_pin_t  sck_pin;
    board_pin_t  miso_pin;
    board_pin_t  mosi_pin;
    board_pin_t  nss_pin;      /**< Set .pin = 0 for software NSS      */
    IRQn_Type    irqn;
    uint32_t     irq_priority;
} board_spi_desc_t;

typedef struct {
    uint8_t       dev_id;
    const char   *label;
    GPIO_TypeDef *port;
    uint16_t      pin;
    uint32_t      mode;
    uint32_t      pull;
    uint32_t      speed;
    uint8_t       active_state;  /**< GPIO_PIN_SET or GPIO_PIN_RESET   */
    uint8_t       initial_state; /**< 0 = inactive, 1 = active at boot */
} board_gpio_desc_t;

typedef struct {
    const char              *board_name;
    const board_uart_desc_t *uart_table;
    uint8_t                  uart_count;
    uint8_t                  uart_shell_id;
    const board_iic_desc_t  *iic_table;
    uint8_t                  iic_count;
    const board_spi_desc_t  *spi_table;
    uint8_t                  spi_count;
    const board_gpio_desc_t *gpio_table;
    uint8_t                  gpio_count;
} board_config_t;

const board_config_t    *board_get_config(void);
const board_uart_desc_t *board_find_uart(USART_TypeDef *instance);
const board_iic_desc_t  *board_find_iic(I2C_TypeDef *instance);
const board_spi_desc_t  *board_find_spi(SPI_TypeDef *instance);
void  board_clk_enable(void);
uint8_t board_get_shell_uart_id(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_BOARD_BOARD_CONFIG_H_ */
