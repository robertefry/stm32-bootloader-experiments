
PROJECT_ROOT := ..

BINARY := bootloader
FLASH_OFFSET := 0x08000000

SRC_DIR := src
INC_DIR := include

# Source Files
SRCS += $(SRC_DIR)/main.c

################################################################################

all: elf bin lst

include ../mk/dep/common.mk
include ../mk/dep/libopencm3.mk

include ../mk/platform/nucleo-f446re.mk
include ../mk/build.mk
include ../mk/flash.mk
