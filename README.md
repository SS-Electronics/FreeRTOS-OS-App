# FreeRTOS-OS-App

Root workspace for the **FreeRTOS-OS** embedded OS project.  
The OS kernel lives in `FreeRTOS-OS/`; application code lives in `app/`.

See [FreeRTOS-OS/README.md](FreeRTOS-OS/README.md) for full documentation, architecture overview, and API reference.

---

## Quick Start

```bash
# 1. Clone with submodules
git clone --recurse-submodules <repo-url>
cd FreeRTOS-OS-App

# 2. Install prerequisites (one-time)
cd FreeRTOS-OS && make install-prerequisites && cd ..

# 3. Configure target MCU (one-time, or when switching hardware)
make menuconfig      # Kconfig ncurses menu
make config-outputs  # generates FreeRTOS-OS/config/autoconf.h

# 4. Generate BSP and IRQ table from board XML (one-time, or after XML changes)
make board-gen       # generates app/board/{board_config.c, board_device_ids.h, …}
make irq_gen         # generates irq_table.c + irq_hw_init_generated.c

# 5. Build OS + application
make app             # → build/app.elf

# 6. Flash
make flash-app
```

> Steps 2–4 are one-time setup. After that, only steps 5–6 are needed for daily builds.

---

## VS Code Workflow

Open this folder (`FreeRTOS-OS-App/`) in VS Code — **not** the `FreeRTOS-OS/` subfolder.

| Action | How |
|---|---|
| Build | `Ctrl+Shift+B` (default build task) |
| Flash + Debug | `F5` → Auto — Build, Flash & Debug |
| Flash only (no debug) | `F5` → Auto — Flash Only (Run) |
| Attach to running target | `F5` → Auto — Attach |
| Configure MCU | Terminal → Run Task → `menuconfig` |
| Install debug tools | Terminal → Run Task → `install-debug-tools` |

---

## Directory Layout

```
FreeRTOS-OS-App/
├── scripts/              Code generators (gen_irq_table.py, gen_board_config.py …)
├── FreeRTOS-OS/          OS kernel, drivers, services, HAL
│   ├── arch/             CPU/MCU architecture, linker scripts, OpenOCD configs
│   ├── config/           autoconf.h, FreeRTOSConfig.h (generated + hand-written)
│   ├── docs/             Documentation (DRIVERS.md, IRQ.md, BOARD.md …)
│   ├── drivers/          Generic driver layer (UART, SPI, I2C, GPIO)
│   │   └── hal/stm32/    STM32 HAL backends + NVIC irq_chip
│   ├── init/             main() — HAL init, clock config, scheduler start
│   ├── ipc/              Message queues, ring-buffer
│   ├── irq/              IRQ descriptor chain (irq_desc.c, irq_notify.c, irq_table.c)
│   ├── kernel/           Thread API, FreeRTOS-Kernel submodule
│   ├── mm/               kmalloc/kfree, intrusive linked list
│   └── services/         Management service threads (uart_mgmt, gpio_mgmt …)
└── app/                  User application
    ├── app_main.c        Tasks + UART echo + EXTI button example
    └── board/
        ├── stm32f411_devboard.xml       Board peripheral description
        ├── irq_table.xml                IRQ ID → NVIC priority mapping
        ├── irq_hw_init_generated.c/.h   Generated — irq_hw_init_all()
        ├── board_config.c/.h            Generated — HAL handles + descriptors
        └── board_device_ids.h           Generated — UART_DEBUG, BTN_USER, …
```

---

## Prerequisites

See [FreeRTOS-OS/README.md § Prerequisites](FreeRTOS-OS/README.md#prerequisites) for the full install guide including ST-Link udev rules and VS Code extension setup.

```bash
cd FreeRTOS-OS && make install-prerequisites
```

---

## License

GNU General Public License v3.0 — see `COPYING` or <https://www.gnu.org/licenses/gpl-3.0.html>.

*Author: Subhajit Roy — subhajitroy005@gmail.com*
