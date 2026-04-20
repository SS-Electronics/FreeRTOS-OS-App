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
cd FreeRTOS-OS
make install-prerequisites
cd ..

# 3. Configure target MCU (one-time, or when switching hardware)
cd FreeRTOS-OS && make menuconfig && cd ..

# 4. Generate autoconf.h from .config
cd FreeRTOS-OS && make config-outputs && cd ..

# 5. Build OS + application
cd FreeRTOS-OS && make all APP_DIR=../app TARGET_NAME=app && cd ..

# 6. Flash
cd FreeRTOS-OS && make flash && cd ..
```

> Steps 2–4 are one-time setup. After that, only step 5–6 are needed.

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
├── FreeRTOS-OS/          OS kernel, drivers, services, HAL
│   ├── arch/             CPU/MCU architecture, linker scripts, OpenOCD configs
│   ├── drivers/          Generic driver layer (UART, SPI, I2C, GPIO)
│   ├── init/             main() — HAL init, clock config, scheduler start
│   ├── ipc/              Message queues, ring-buffer
│   ├── kernel/           Thread API, FreeRTOS-Kernel submodule
│   ├── mm/               kmalloc/kfree, intrusive linked list
│   ├── scripts/          Install scripts, board/debug code generators
│   └── services/         Management service threads (uart_mgmt, gpio_mgmt …)
└── app/                  User application (app_main.c, board XML, board BSP)
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
