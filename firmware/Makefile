
PROJECT_ROOT := ..

BINARY := firmware

FLASH_ORIGIN := 0x08008000
FLASH_LENGTH := 480K

SRC_DIR := src
INC_DIR := include

# Source Files
SRCS += $(SRC_DIR)/main.c
SRCS += $(SRC_DIR)/systick.c

################################################################################

all: elf bin lst

include ../mk/dep/common.mk
include ../mk/dep/libopencm3.mk

include ../mk/platform/nucleo-f446re.mk
include ../mk/build.mk
include ../mk/flash.mk
