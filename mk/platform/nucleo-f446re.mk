
ifndef PROJECT_ROOT
  $(error "Variable PROJECT_ROOT is not defined.")
endif

ifndef FLASH_ORIGIN
  $(error "Variable FLASH_ORIGIN is not defined.")
endif
ifndef FLASH_LENGTH
  $(error "Variable FLASH_LENGTH is not defined.")
endif

################################################################################
# Hardware Configuration (STM32F446RET6)

PLATFORM := nucleo-f446re

ARCH_FLAGS := -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

CC_FLAGS += $(ARCH_FLAGS) -DSTM32F4

LD_FLAGS += $(ARCH_FLAGS) -T$(PROJECT_ROOT)/ld/$(PLATFORM).ld
LD_FLAGS += -Wl,--defsym=FLASH_ORIGIN=$(FLASH_ORIGIN)
LD_FLAGS += -Wl,--defsym=FLASH_LENGTH=$(FLASH_LENGTH)
