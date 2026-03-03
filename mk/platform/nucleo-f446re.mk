
################################################################################
# Hardware Configuration (STM32F446RET6)

PLATFORM := nucleo-f446re

ARCH_FLAGS := -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

CC_FLAGS += $(ARCH_FLAGS) -DSTM32F4
LD_FLAGS += $(ARCH_FLAGS) -T$(PLATFORM).ld
