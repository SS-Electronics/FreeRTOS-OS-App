# File:   Makefile  (top-level)
# Module: FreeRTOS-OS-App build entry point
#
# One-time setup (do these once, or whenever config / board XML changes):
# ───────────────────────────────────────────────────────────────────────
#   make menuconfig          Configure kernel (generates FreeRTOS-OS/.config)
#   make config-outputs      Generate autoconf.mk and autoconf.h from .config
#   make board-gen           Generate BSP from boards/<CONFIG_BOARD>.xml
#
# Build (after setup):
# ───────────────────────────────────────────────────────────────────────
#   make kernel              Build FreeRTOS-OS standalone  → build/kernel.elf
#   make app                 Build FreeRTOS-OS + app/       → build/app.elf
#   make flash-kernel        Flash kernel.elf via OpenOCD
#   make flash-app           Flash app.elf via OpenOCD
#   make clean               Remove build artefacts
#
# Board override (default: stm32f411_devboard):
#   make board-gen CONFIG_BOARD=my_board
#   make app       CONFIG_BOARD=my_board

OS_DIR  := FreeRTOS-OS
APP_SRC := app

# CONFIG_BOARD and other command-line overrides are forwarded to sub-makes
# automatically by make via MAKEFLAGS — no explicit pass-through needed.

.PHONY: all kernel app flash-kernel flash-app clean
.PHONY: menuconfig config-outputs board-gen irq_gen

all: app

# ── Standalone OS build ──────────────────────────────────────────────
kernel:
	$(MAKE) -C $(OS_DIR) all TARGET_NAME=kernel

# ── OS + application build ───────────────────────────────────────────
app:
	$(MAKE) -C $(OS_DIR) all APP_DIR=../$(APP_SRC) TARGET_NAME=app

# ── Flash targets ────────────────────────────────────────────────────
flash-kernel:
	$(MAKE) -C $(OS_DIR) flash TARGET_NAME=kernel

flash-app:
	$(MAKE) -C $(OS_DIR) flash APP_DIR=../$(APP_SRC) TARGET_NAME=app

# ── One-time setup ───────────────────────────────────────────────────
menuconfig:
	$(MAKE) -C $(OS_DIR) menuconfig

config-outputs:
	$(MAKE) -C $(OS_DIR) config-outputs

board-gen:
	$(MAKE) -C $(OS_DIR) board-gen APP_DIR=../$(APP_SRC)

# ── IRQ table code generation ─────────────────────────────────────────
# Reads app/board/irq_table.xml and writes:
#   FreeRTOS-OS/irq/irq_table.c            — software IRQ name table
#   app/board/irq_hw_init_generated.c/.h   — NVIC priority / binding init
irq_gen:
	python3 $(OS_DIR)/scripts/gen_irq_table.py $(APP_SRC)/board/irq_table.xml

# ── Housekeeping ─────────────────────────────────────────────────────
clean:
	$(MAKE) -C $(OS_DIR) clean
