
# TODO: Hide compile output when not in verbose mode.

ifndef PROJECT_ROOT
  $(error "Variable PROJECT_ROOT is not defined.")
endif

ifndef PLATFORM
  $(error "Variable PLATFORM is not defined.")
endif
ifndef BINARY
  $(error "Variable BINARY is not defined.")
endif

ifndef INC_DIR
  $(error "Variable INC_DIR is not defined.")
endif
ifndef SRC_DIR
  $(error "Variable SRC_DIR is not defined.")
endif
ifndef SRCS
  $(error "Variable SRCS is not defined.")
endif

################################################################################
# Build Mode

BUILD ?= debug
BUILD_DIR ?= $(PROJECT_ROOT)/$(BINARY)/.build

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

# TODO: Use more compiler warnings against bad code.
CC_FLAGS += -Wall -Wextra -Wshadow -Wundef
CC_FLAGS += -Wimplicit-function-declaration -Wredundant-decls
CC_FLAGS += -Wstrict-prototypes

LD_FLAGS += --static -nostartfiles
LD_FLAGS += -Wl,-gc-sections
LD_FLAGS += -Wl,-Map=$(BUILD_DIR)/$(BUILD)/$(*).map -Wl,--cref
LD_FLAGS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

################################################################################

OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump

# TODO: Object .S files as well as .c files
CC_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/$(BUILD)/%.o,$(SRCS))
LD_LIBS = $(addprefix -l,$(LD_LIB_NAMES))

elf: $(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).elf
bin: $(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).bin
lst: $(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).lst

clean:
	$(RM) -dr $(BUILD_DIR)

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
