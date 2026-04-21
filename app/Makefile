# File:   app/Makefile
# Module: Application build fragment
# Info:   Included by FreeRTOS-OS/Makefile when APP_DIR is set.
#         Declares app object files (app-obj-y) and extra include paths
#         (APP_INCLUDES).  All paths are relative to APP_DIR.

# Add source files here — one .o per .c
app-obj-y += app_main.o
app-obj-y += board/board_config.o
app-obj-y += board/irq_hw_init_generated.o
app-obj-y += board/irq_periph_dispatch_generated.o

# Expose app headers to the compiler
APP_INCLUDES += -I$(APP_DIR)
APP_INCLUDES += -I$(APP_DIR)/os_conf_include
