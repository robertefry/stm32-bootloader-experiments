
# TODO: Hide compile output when not in verbose mode.

BUILD_DIR := .build

SRC_DIR := src
INC_DIR := include

# Source Files
SRCS += $(SRC_DIR)/main.c
SRCS += $(SRC_DIR)/systick.c

################################################################################
# Hardware Configuration (STM32F446RET6)

BINARY := nucleo-f446re

ARCH_FLAGS := -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

CC_FLAGS += $(ARCH_FLAGS) -DSTM32F4
LD_FLAGS += $(ARCH_FLAGS) -T$(BINARY).ld

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
# Build Mode

BUILD ?= debug
BUILD_SET := false

ifeq ($(BUILD),release)
  CC_FLAGS += -D NDEBUG
  BUILD_SET := true
endif

ifeq ($(BUILD),debug)
  CC_FLAGS += -ggdb3
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

CC_FLAGS += -std=c17 -Os -MD -I$(INC_DIR)
CC_FLAGS += -fno-common -ffunction-sections -fdata-sections

CC_FLAGS += -Wall -Wextra -Wshadow -Wundef
CC_FLAGS += -Wimplicit-function-declaration -Wredundant-decls
CC_FLAGS += -Wstrict-prototypes

LD_FLAGS += --static -nostartfiles
LD_FLAGS += -Wl,-gc-sections #-Wl,--print-gc-sections
# TODO: Print GC sections when in verbose mode.

LD_FLAGS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

################################################################################
################################################################################

OBJCOPY := arm-none-eabi-objcopy

CC_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
LD_LIBS = $(addprefix -l,$(LD_LIB_NAMES))

OUT_DIR := $(BUILD_DIR)/$(MODE)

all: bin elf hex

elf: $(OUT_DIR)/$(BINARY).elf
bin: $(OUT_DIR)/$(BINARY).bin
hex: $(OUT_DIR)/$(BINARY).hex

clean:
	$(RM) -dr $(BUILD_DIR)

$(OUT_DIR)/%.elf: $(CC_OBJS) $(LD_LIB_PATHS) | $(OUT_DIR)
	$(LD) $(LD_FLAGS) -Wl,-Map=$(OUT_DIR)/$(*).map -Wl,--cref $(CC_OBJS) $(LD_LIBS) -o $@

$(OUT_DIR)/%.bin: $(OUT_DIR)/%.elf | $(OUT_DIR)
	$(OBJCOPY) -O binary $< $@

$(OUT_DIR)/%.hex: $(OUT_DIR)/%.elf | $(OUT_DIR)
	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S | $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

-include $(CC_OBJS:.o=.d)

.PHONY: all elf bin hex
.DEFAULT_GOAL := all
