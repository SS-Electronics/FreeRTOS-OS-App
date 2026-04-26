/*
 * AUTO-GENERATED — DO NOT EDIT
 * Generator : scripts/gen_board_config.py
 * Source    : ../app/board/stm32f411_devboard.xml
 * Date      : 2026-04-26
 *
 * Re-generate:
 *   python3 scripts/gen_board_config.py ../app/board/stm32f411_devboard.xml
 */

#include <board/board_config.h>
#include <board/board_device_ids.h>

/* ── CMSIS system variables ─────────────────────────────────────────────── */
/* Placed in .boot_data: valid before the .data copy in Reset_Handler.      */
#if (CONFIG_DEVICE_VARIANT == MCU_VAR_STM)
#include <def_attributes.h>
__SECTION_BOOT_DATA uint32_t SystemCoreClock = BOARD_SYSCLK_HZ;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};
#endif /* CONFIG_DEVICE_VARIANT == MCU_VAR_STM */

/* ── HAL peripheral handles ─────────────────────────────────────────────── */
#ifdef HAL_UART_MODULE_ENABLED
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
#endif /* HAL_UART_MODULE_ENABLED */
#ifdef HAL_I2C_MODULE_ENABLED
I2C_HandleTypeDef hi2c1;
#endif /* HAL_I2C_MODULE_ENABLED */
#ifdef HAL_SPI_MODULE_ENABLED
SPI_HandleTypeDef hspi1;
#endif /* HAL_SPI_MODULE_ENABLED */

/* ── UART table ─────────────────────────────────────────────────────────── */
#ifdef HAL_UART_MODULE_ENABLED
static const board_uart_desc_t _uart_table[BOARD_UART_COUNT] = {
    /* UART_DEBUG → USART1 */
    [0] = {
        .dev_id       = 0,
        .instance     = USART1,
        .baudrate     = 115200,
        .word_len     = UART_WORDLENGTH_8B,
        .stop_bits    = UART_STOPBITS_1,
        .parity       = UART_PARITY_NONE,
        .mode         = UART_MODE_TX_RX,
        .tx_pin       = {
            .port      = GPIOA,
            .pin       = GPIO_PIN_9,
            .mode      = GPIO_MODE_AF_PP,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF7_USART1,
        },
        .rx_pin       = {
            .port      = GPIOA,
            .pin       = GPIO_PIN_10,
            .mode      = GPIO_MODE_AF_PP,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF7_USART1,
        },
        .irqn         = USART1_IRQn,
        .irq_priority = 5,
    },
    /* UART_APP → USART2 */
    [1] = {
        .dev_id       = 1,
        .instance     = USART2,
        .baudrate     = 115200,
        .word_len     = UART_WORDLENGTH_8B,
        .stop_bits    = UART_STOPBITS_1,
        .parity       = UART_PARITY_NONE,
        .mode         = UART_MODE_TX_RX,
        .tx_pin       = {
            .port      = GPIOA,
            .pin       = GPIO_PIN_2,
            .mode      = GPIO_MODE_AF_PP,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF7_USART2,
        },
        .rx_pin       = {
            .port      = GPIOA,
            .pin       = GPIO_PIN_3,
            .mode      = GPIO_MODE_AF_PP,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF7_USART2,
        },
        .irqn         = USART2_IRQn,
        .irq_priority = 5,
    },
};
#define _UART_TABLE  _uart_table
#else
#define _UART_TABLE  NULL
#undef  BOARD_UART_COUNT
#define BOARD_UART_COUNT 0
#endif /* HAL_UART_MODULE_ENABLED */

/* ── I2C table ──────────────────────────────────────────────────────────── */
#ifdef HAL_I2C_MODULE_ENABLED
static const board_iic_desc_t _iic_table[BOARD_IIC_COUNT] = {
    /* I2C_SENSOR_BUS → I2C1 */
    [0] = {
        .dev_id       = 0,
        .instance     = I2C1,
        .clock_hz     = 400000,
        .addr_mode    = I2C_ADDRESSINGMODE_7BIT,
        .dual_addr    = I2C_DUALADDRESS_DISABLE,
        .scl_pin      = {
            .port      = GPIOB,
            .pin       = GPIO_PIN_6,
            .mode      = GPIO_MODE_AF_OD,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF4_I2C1,
        },
        .sda_pin      = {
            .port      = GPIOB,
            .pin       = GPIO_PIN_7,
            .mode      = GPIO_MODE_AF_OD,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF4_I2C1,
        },
        .ev_irqn      = I2C1_EV_IRQn,
        .er_irqn      = I2C1_ER_IRQn,
        .irq_priority = 6,
    },
};
#define _IIC_TABLE  _iic_table
#else
#define _IIC_TABLE  NULL
#undef  BOARD_IIC_COUNT
#define BOARD_IIC_COUNT 0
#endif /* HAL_I2C_MODULE_ENABLED */

/* ── SPI table ──────────────────────────────────────────────────────────── */
#ifdef HAL_SPI_MODULE_ENABLED
static const board_spi_desc_t _spi_table[BOARD_SPI_COUNT] = {
    /* SPI_FLASH_BUS → SPI1 */
    [0] = {
        .dev_id         = 0,
        .instance       = SPI1,
        .mode           = SPI_MODE_MASTER,
        .direction      = SPI_DIRECTION_2LINES,
        .data_size      = SPI_DATASIZE_8BIT,
        .clk_polarity   = SPI_POLARITY_LOW,
        .clk_phase      = SPI_PHASE_1EDGE,
        .nss            = SPI_NSS_SOFT,
        .baud_prescaler = SPI_BAUDRATEPRESCALER_16,
        .bit_order      = SPI_FIRSTBIT_MSB,
        .sck_pin        = {
            .port      = GPIOA,
            .pin       = GPIO_PIN_5,
            .mode      = GPIO_MODE_AF_PP,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF5_SPI1,
        },
        .miso_pin       = {
            .port      = GPIOA,
            .pin       = GPIO_PIN_6,
            .mode      = GPIO_MODE_AF_PP,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF5_SPI1,
        },
        .mosi_pin       = {
            .port      = GPIOA,
            .pin       = GPIO_PIN_7,
            .mode      = GPIO_MODE_AF_PP,
            .pull      = GPIO_NOPULL,
            .speed     = GPIO_SPEED_FREQ_VERY_HIGH,
            .alternate = GPIO_AF5_SPI1,
        },
        .nss_pin        = { .pin = 0 },  /* software NSS */
        .irqn           = SPI1_IRQn,
        .irq_priority   = 6,
    },
};
#define _SPI_TABLE  _spi_table
#else
#define _SPI_TABLE  NULL
#undef  BOARD_SPI_COUNT
#define BOARD_SPI_COUNT 0
#endif /* HAL_SPI_MODULE_ENABLED */

/* ── GPIO table ─────────────────────────────────────────────────────────── */
static const board_gpio_desc_t _gpio_table[BOARD_GPIO_COUNT] = {
    /* LED_BOARD */
    [0] = {
        .dev_id        = 0,
        .label         = "LED_BOARD",
        .port          = GPIOC,
        .pin           = GPIO_PIN_13,
        .mode          = GPIO_MODE_OUTPUT_PP,
        .pull          = GPIO_NOPULL,
        .speed         = GPIO_SPEED_FREQ_LOW,
        .active_state  = GPIO_PIN_RESET,
        .initial_state = 0,
    },
    /* BTN_USER */
    [1] = {
        .dev_id        = 1,
        .label         = "BTN_USER",
        .port          = GPIOA,
        .pin           = GPIO_PIN_0,
        .mode          = GPIO_MODE_IT_RISING,
        .pull          = GPIO_PULLDOWN,
        .speed         = GPIO_SPEED_FREQ_LOW,
        .active_state  = GPIO_PIN_SET,
        .initial_state = 0,
    },
    /* LED_STATUS */
    [2] = {
        .dev_id        = 2,
        .label         = "LED_STATUS",
        .port          = GPIOB,
        .pin           = GPIO_PIN_0,
        .mode          = GPIO_MODE_OUTPUT_PP,
        .pull          = GPIO_NOPULL,
        .speed         = GPIO_SPEED_FREQ_LOW,
        .active_state  = GPIO_PIN_SET,
        .initial_state = 0,
    },
};

/* ── Top-level board config ─────────────────────────────────────────────── */
static const board_config_t g_board_config = {
    .board_name    = "stm32f411_devboard",
    .uart_table    = _UART_TABLE,
    .uart_count    = BOARD_UART_COUNT,
    .uart_shell_id = UART_DEBUG,  /* role="shell" */
    .iic_table     = _IIC_TABLE,
    .iic_count     = BOARD_IIC_COUNT,
    .spi_table     = _SPI_TABLE,
    .spi_count     = BOARD_SPI_COUNT,
    .gpio_table    = _gpio_table,
    .gpio_count    = BOARD_GPIO_COUNT,
};

/* ═══════════════════════════════════════════════════════════════════════════
 * Board API implementation
 * ═══════════════════════════════════════════════════════════════════════════ */

const board_config_t *board_get_config(void)    { return &g_board_config; }
uint8_t board_get_shell_uart_id(void)           { return g_board_config.uart_shell_id; }

const board_uart_desc_t *board_find_uart(USART_TypeDef *instance)
{
    for (uint8_t i = 0; i < g_board_config.uart_count; i++)
        if (g_board_config.uart_table && g_board_config.uart_table[i].instance == instance)
            return &g_board_config.uart_table[i];
    return NULL;
}

const board_iic_desc_t *board_find_iic(I2C_TypeDef *instance)
{
    for (uint8_t i = 0; i < g_board_config.iic_count; i++)
        if (g_board_config.iic_table && g_board_config.iic_table[i].instance == instance)
            return &g_board_config.iic_table[i];
    return NULL;
}

const board_spi_desc_t *board_find_spi(SPI_TypeDef *instance)
{
    for (uint8_t i = 0; i < g_board_config.spi_count; i++)
        if (g_board_config.spi_table && g_board_config.spi_table[i].instance == instance)
            return &g_board_config.spi_table[i];
    return NULL;
}

void board_clk_enable(void)
{
    /* System bus clocks */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

#ifdef HAL_UART_MODULE_ENABLED
    /* UART peripheral clocks */
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();
#endif /* HAL_UART_MODULE_ENABLED */

#ifdef HAL_I2C_MODULE_ENABLED
    /* I2C peripheral clocks */
    __HAL_RCC_I2C1_CLK_ENABLE();
#endif /* HAL_I2C_MODULE_ENABLED */

#ifdef HAL_SPI_MODULE_ENABLED
    /* SPI peripheral clocks */
    __HAL_RCC_SPI1_CLK_ENABLE();
#endif /* HAL_SPI_MODULE_ENABLED */

    /* GPIO port clocks for alternate-function peripheral pins */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
}

