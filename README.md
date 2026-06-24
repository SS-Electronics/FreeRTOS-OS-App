# App — FreeRTOS-OS Application (stm32h723)

Scaffolded from the **stm32h723** example.
MCU: `#   CONFIG_TARGET_MCU     → "STM32H723xx"  (H7 HAL, ARM_CM7/r0p1 portable)`  |  CPU: `ARM-CM7`  |  Board config: `app/board/stm32h723_devboard.xml`

---

## Quick start

```bash
cd FreeRTOS-OS

# Generate board BSP + activate Kconfig + compile
make dev-stm32h723 APP_DIR=../app

# Flash to target
make dev-stm32h723-flash
```

> **Tip:** pass `DEBUG=0` for a release build (-Os -DNDEBUG):
> ```bash
> make dev-stm32h723 APP_DIR=../app DEBUG=0
> ```

---

## Directory layout

```
.
├── app/                               Application project (this scaffold)
│   ├── app_main.c                     Entry point — implement your tasks here
│   ├── Makefile                       Build fragment (app-obj-y, APP_INCLUDES)
│   ├── kconfig.conf                   Kconfig preset
│   ├── board/
│   │   ├── stm32h723_devboard.xml   Board peripheral descriptor (edit this)
│   │   ├── irq_table.xml              IRQ routing table
│   │   ├── mcu_config.h               MCU peripheral counts / UART_x_EN flags
│   │   └── <generated files>          board_config.{c,h}, board_device_ids.h, …
│   └── os_conf_include/
│       ├── conf_board.h               COMM_PRINTK_HW_ID selection
│       └── def_compiler.h             Compiler / type includes
└── FreeRTOS-OS/                       RTOS kernel (this repo)
```

---

## Build reference

| Command | Description |
|---------|-------------|
| `make dev-stm32h723 APP_DIR=../app` | Full build: gen → config → compile |
| `make dev-stm32h723 APP_DIR=../app DEBUG=0` | Release build (-Os -DNDEBUG) |
| `make dev-stm32h723-flash` | Flash via OpenOCD / ST-Link |
| `make dev-stm32h723-gen` | Regenerate board BSP files only |
| `make dev-stm32h723-clean` | Remove generated files + build/ |
| `make board-gen APP_DIR=../app` | Regenerate BSP from board XML |

---

## Customising

### Add application source files

Edit `app/Makefile`:
```makefile
app-obj-y += my_module.o
app-obj-y += subdir/another.o
```
Add the corresponding `.c` files under `app/`.

### Change board peripherals

1. Edit `app/board/stm32h723_devboard.xml`.
2. Edit `app/board/irq_table.xml` if IRQ routing changes.
3. Regenerate:
   ```bash
   cd FreeRTOS-OS
   make dev-stm32h723-gen APP_DIR=../app
   ```

### Change Kconfig (HAL modules, heap size, …)

1. Edit `app/kconfig.conf`.
2. Activate:
   ```bash
   cd FreeRTOS-OS
   cp ../app/kconfig.conf .config && make config-outputs
   ```
3. Rebuild: `make dev-stm32h723 APP_DIR=../app`

---

## Available examples

Re-scaffold to switch target board:

```bash
cd FreeRTOS-OS
make setup-project EXAMPLE=stm32h723   # NUCLEO-H723ZG  (Cortex-M7, 1 MB flash)
make setup-project EXAMPLE=stm32f411   # STM32F411 devboard (Cortex-M4F, 512 KB)
make setup-project EXAMPLE=stm32u575   # NUCLEO-U575ZI-Q (Cortex-M33 + TrustZone)
```
