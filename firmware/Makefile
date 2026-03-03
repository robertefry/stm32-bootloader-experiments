
# TODO: Hide compile output when not in verbose mode.

BINARY := firmware

SRC_DIR := src
INC_DIR := include

# Source Files
SRCS += $(SRC_DIR)/main.c
SRCS += $(SRC_DIR)/systick.c

################################################################################
# Dep: Common

LIB_COMMON_DIR = ../common
CC_FLAGS += -I$(LIB_COMMON_DIR)/include

################################################################################
# Dep: LibOpenCM3

LIB_OPENCM3_NAME = opencm3_stm32f4
LIB_OPENCM3_DIR  = ../deps/libopencm3
LIB_OPENCM3      = $(LIB_OPENCM3_DIR)/lib/lib$(LIB_OPENCM3_NAME).a

$(LIB_OPENCM3):
	$(MAKE) -C $(LIB_OPENCM3_DIR)

LD_LIB_NAMES += $(LIB_OPENCM3_NAME)
LD_LIB_PATHS += $(LIB_OPENCM3)

LD_FLAGS += -L$(LIB_OPENCM3_DIR)/lib
CC_FLAGS += -I$(LIB_OPENCM3_DIR)/include

################################################################################
# Hardware Configuration (STM32F446RET6)

PLATFORM := nucleo-f446re

ARCH_FLAGS := -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

CC_FLAGS += $(ARCH_FLAGS) -DSTM32F4
LD_FLAGS += $(ARCH_FLAGS) -T$(PLATFORM).ld

################################################################################
# Build Mode

BUILD ?= debug
BUILD_DIR ?= .build

BUILD_SET := false

ifeq ($(BUILD),release)
  CC_FLAGS += -Os -DNDEBUG
  BUILD_SET := true
endif

ifeq ($(BUILD),debug)
  CC_FLAGS += -Og -ggdb3
  BUILD_SET := true
endif

ifeq ($(BUILD_SET),false)
  $(error Unknown BUILD type '$(BUILD)'.)
endif

################################################################################
# Compiler & Linker

CC    := arm-none-eabi-gcc
LD    := arm-none-eabi-gcc
DB    := arm-none-eabi-gdb

CC_FLAGS += -std=c17 -MD -I$(INC_DIR)
CC_FLAGS += -fno-common -ffunction-sections -fdata-sections

CC_FLAGS += -Wall -Wextra -Wshadow -Wundef
CC_FLAGS += -Wimplicit-function-declaration -Wredundant-decls
CC_FLAGS += -Wstrict-prototypes

LD_FLAGS += --static -nostartfiles
LD_FLAGS += -Wl,-gc-sections
LD_FLAGS += -Wl,-Map=$(BUILD_DIR)/$(BUILD)/$(*).map -Wl,--cref
LD_FLAGS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

################################################################################
################################################################################

OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
STFLASH := st-flash

CC_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/$(BUILD)/%.o,$(SRCS))
LD_LIBS = $(addprefix -l,$(LD_LIB_NAMES))

all: elf bin lst
.DEFAULT_GOAL := all

clean:
	$(RM) -dr $(BUILD_DIR)

elf: $(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).elf
bin: $(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).bin
lst: $(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).lst

flash: bin
	$(STFLASH) write "$(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).bin" 0x08008000

$(BUILD_DIR)/$(BUILD)/%.elf: $(CC_OBJS) $(LD_LIB_PATHS) | $(BUILD_DIR)/$(BUILD)
	$(LD) $(LD_FLAGS) $(CC_OBJS) $(LD_LIBS) -o $@

$(BUILD_DIR)/$(BUILD)/%.bin: $(BUILD_DIR)/$(BUILD)/%.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(BUILD)/%.lst: $(BUILD_DIR)/$(BUILD)/%.elf
	$(OBJDUMP) -D $< > $@

$(BUILD_DIR)/$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/$(BUILD)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/$(BUILD)/%.o: $(SRC_DIR)/%.S | $(BUILD_DIR)/$(BUILD)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/$(BUILD):
	mkdir -p $@
