/*
 * board_config.h — Board peripheral configuration (compile-time device table)
 *
 * This file is part of FreeRTOS-OS Project.
 *
 * Design
 * ──────
 * Analogous to a Linux Device Tree, but resolved entirely at compile time as
 * C constant structs.  Every board declares one const board_config_t that
 * contains tables of peripheral descriptors (UART, I2C, SPI, GPIO).
 *
 * Drivers and MSP init functions query this single source of truth instead of
 * having pin assignments and peripheral parameters scattered across multiple
 * source files.
 *
 * Porting to a new board
 * ──────────────────────
 *   1. Create  boards/<board_name>/board_config.c
 *   2. Define  g_board_config  with your peripheral tables
 *   3. Pass    CONFIG_BOARD=<board_name>  to make  (or edit boards/Makefile)
 *   4. Rebuild — no other source files need to change
 *
 * Include path
 * ────────────
 *   #include <board/board_config.h>
 */

#ifndef INCLUDE_BOARD_BOARD_CONFIG_H_
#define INCLUDE_BOARD_BOARD_CONFIG_H_

#include <def_std.h>
#include <device.h>     /* CMSIS types: GPIO_TypeDef, USART_TypeDef, IRQn_Type … */

#ifdef __cplusplus
extern "C" {
#endif

/* ═══════════════════════════════════════════════════════════════════════════
 * 0.  Clock tree configuration  (STM32F411VET6 @ 100 MHz, HSI source)
 *
 *     HSI (16 MHz)
 *       └─ /PLLM (16) ──→ 1 MHz VCO input
 *            └─ ×PLLN (200) ──→ 200 MHz VCO output
 *                 ├─ /PLLP (2)  ──→ SYSCLK = 100 MHz
 *                 └─ /PLLQ (4)  ──→ USB/SDIO =  50 MHz
 *     AHB  = SYSCLK / 1 = 100 MHz
 *     APB1 = HCLK   / 2 =  50 MHz  (F411 max)
 *     APB2 = HCLK   / 1 = 100 MHz
 *
 *     hal_rcc_stm32.c reads these macros to configure the PLL.
 *     CMSIS variables are defined in board_config.c and declared below.
 * ═══════════════════════════════════════════════════════════════════════════ */

/* PLL multiplier / divider values */
#define BOARD_RCC_PLLM          16U             /* HSI/PLLM = 1 MHz VCO input */
#define BOARD_RCC_PLLN          200U            /* VCO output = 200 MHz        */
#define BOARD_RCC_PLLP          RCC_PLLP_DIV2  /* SYSCLK  = 200 / 2 = 100 MHz */
#define BOARD_RCC_PLLQ          4U             /* USB/SDIO = 200 / 4 =  50 MHz */

/* Resulting bus frequencies (informational — drivers query HAL at runtime)   */
#define BOARD_SYSCLK_HZ         100000000UL
#define BOARD_APB1_HZ            50000000UL
#define BOARD_APB2_HZ           100000000UL

/* Flash wait-states for 90–100 MHz, VDD 2.7–3.6 V (STM32F411 RM §3.4)      */
#define BOARD_FLASH_LATENCY     FLASH_LATENCY_3

/* CMSIS system variable declarations — definitions in board_config.c.
 * SystemCoreClock is placed in .boot_data so it is valid before .data copy.  */
extern          uint32_t SystemCoreClock;
extern const uint8_t     AHBPrescTable[16];
extern const uint8_t     APBPrescTable[8];

/* ═══════════════════════════════════════════════════════════════════════════
 * 1.  Pin descriptor
 *     Carries everything HAL_GPIO_Init() needs for one physical pin.
 *     Use alternate = 0 for non-AF (output/input) pins.
 * ═══════════════════════════════════════════════════════════════════════════ */

typedef struct {
    GPIO_TypeDef *port;       /**< GPIOA … GPIOE                              */
    uint16_t      pin;        /**< GPIO_PIN_0 … GPIO_PIN_15                   */
    uint32_t      mode;       /**< GPIO_MODE_AF_PP, GPIO_MODE_OUTPUT_PP, …    */
    uint32_t      pull;       /**< GPIO_NOPULL / GPIO_PULLUP / GPIO_PULLDOWN  */
    uint32_t      speed;      /**< GPIO_SPEED_FREQ_LOW / _MEDIUM / _HIGH …   */
    uint32_t      alternate;  /**< GPIO_AF7_USART1, GPIO_AF4_I2C1, …         */
} board_pin_t;


/* ═══════════════════════════════════════════════════════════════════════════
 * 2.  Peripheral descriptors
 *
 *     Only CMSIS register types (USART_TypeDef*, I2C_TypeDef*, SPI_TypeDef*)
 *     are used here.  These come from stm32f4xx.h and are always available
 *     regardless of which HAL modules are enabled.
 *     Vendor handle types (UART_HandleTypeDef etc.) live in drv_handle.h.
 * ═══════════════════════════════════════════════════════════════════════════ */

/* ── UART / USART ──────────────────────────────────────────────────────── */
typedef struct {
    uint8_t        dev_id;            /**< UART_1 … UART_6  (mcu_config.h)   */
    USART_TypeDef *instance;          /**< USART1, USART2, USART6             */
    uint32_t       baudrate;
    uint32_t       word_len;          /**< UART_WORDLENGTH_8B / _9B           */
    uint32_t       stop_bits;         /**< UART_STOPBITS_1 / _2               */
    uint32_t       parity;            /**< UART_PARITY_NONE / _EVEN / _ODD    */
    uint32_t       mode;              /**< UART_MODE_TX_RX / _TX / _RX        */
    board_pin_t    tx_pin;
    board_pin_t    rx_pin;
    IRQn_Type      irqn;              /**< USARTx_IRQn                        */
    uint32_t       irq_priority;
} board_uart_desc_t;

/* ── I2C ───────────────────────────────────────────────────────────────── */
typedef struct {
    uint8_t      dev_id;              /**< IIC_1 … IIC_3  (mcu_config.h)     */
    I2C_TypeDef *instance;            /**< I2C1, I2C2, I2C3                   */
    uint32_t     clock_hz;            /**< 100000 (standard) or 400000 (fast) */
    uint32_t     addr_mode;           /**< I2C_ADDRESSINGMODE_7BIT / _10BIT   */
    uint32_t     dual_addr;           /**< I2C_DUALADDRESS_DISABLE / _ENABLE  */
    board_pin_t  scl_pin;
    board_pin_t  sda_pin;
    IRQn_Type    ev_irqn;             /**< I2Cx_EV_IRQn                       */
    IRQn_Type    er_irqn;             /**< I2Cx_ER_IRQn                       */
    uint32_t     irq_priority;
} board_iic_desc_t;

/* ── SPI ───────────────────────────────────────────────────────────────── */
typedef struct {
    uint8_t      dev_id;              /**< Logical SPI index                  */
    SPI_TypeDef *instance;            /**< SPI1, SPI2, …                      */
    uint32_t     mode;                /**< SPI_MODE_MASTER / _SLAVE           */
    uint32_t     direction;           /**< SPI_DIRECTION_2LINES / _1LINE      */
    uint32_t     data_size;           /**< SPI_DATASIZE_8BIT / _16BIT         */
    uint32_t     clk_polarity;        /**< SPI_POLARITY_LOW / _HIGH           */
    uint32_t     clk_phase;           /**< SPI_PHASE_1EDGE / _2EDGE           */
    uint32_t     nss;                 /**< SPI_NSS_SOFT / _HARD_OUTPUT        */
    uint32_t     baud_prescaler;      /**< SPI_BAUDRATEPRESCALER_2 … _256     */
    uint32_t     bit_order;           /**< SPI_FIRSTBIT_MSB / _LSB            */
    board_pin_t  sck_pin;
    board_pin_t  miso_pin;
    board_pin_t  mosi_pin;
    board_pin_t  nss_pin;             /**< Set .pin = 0 for software NSS      */
    IRQn_Type    irqn;
    uint32_t     irq_priority;
} board_spi_desc_t;

/* ── General-purpose GPIO line ─────────────────────────────────────────── */
typedef struct {
    uint8_t       dev_id;             /**< Logical GPIO ID → gpio_mgmt        */
    const char   *label;              /**< e.g. "LED_RED", "nRESET"           */
    GPIO_TypeDef *port;
    uint16_t      pin;
    uint32_t      mode;
    uint32_t      pull;
    uint32_t      speed;
    uint8_t       active_state;       /**< GPIO_PIN_SET or GPIO_PIN_RESET     */
    uint8_t       initial_state;      /**< 0 = inactive, 1 = active at boot   */
} board_gpio_desc_t;


/* ═══════════════════════════════════════════════════════════════════════════
 * 3.  Top-level board configuration struct
 * ═══════════════════════════════════════════════════════════════════════════ */

typedef struct {
    const char              *board_name;

    const board_uart_desc_t *uart_table;
    uint8_t                  uart_count;
    uint8_t                  uart_shell_id;  /**< dev_id of the shell CLI UART */

    const board_iic_desc_t  *iic_table;
    uint8_t                  iic_count;

    const board_spi_desc_t  *spi_table;
    uint8_t                  spi_count;

    const board_gpio_desc_t *gpio_table;
    uint8_t                  gpio_count;
} board_config_t;


/* ═══════════════════════════════════════════════════════════════════════════
 * 4.  Board API
 *     Implemented in boards/<board>/board_config.c.
 *     Linked as a single translation unit selected by CONFIG_BOARD.
 * ═══════════════════════════════════════════════════════════════════════════ */

/**
 * @brief  Return a pointer to the active board configuration.
 *         The returned pointer addresses a compile-time constant struct.
 */
const board_config_t    *board_get_config(void);

/**
 * @brief  Find the UART descriptor matching @p instance.
 * @return Pointer into uart_table, or NULL if not found.
 */
const board_uart_desc_t *board_find_uart(USART_TypeDef *instance);

/**
 * @brief  Find the I2C descriptor matching @p instance.
 */
const board_iic_desc_t  *board_find_iic(I2C_TypeDef *instance);

/**
 * @brief  Find the SPI descriptor matching @p instance.
 */
const board_spi_desc_t  *board_find_spi(SPI_TypeDef *instance);

/**
 * @brief  Enable all RCC clocks required by this board: system bus clocks
 *         (SYSCFG, PWR), peripheral clocks (USARTx, I2Cx, SPIx), and GPIO
 *         port clocks.  Call once at boot before any HAL_xxx_Init().
 */
void board_clk_enable(void);

/**
 * @brief  Return the UART dev_id designated as the shell CLI port.
 *         Matches UART_SHELL_HW_ID in conf_os.h.
 */
uint8_t board_get_shell_uart_id(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_BOARD_BOARD_CONFIG_H_ */
