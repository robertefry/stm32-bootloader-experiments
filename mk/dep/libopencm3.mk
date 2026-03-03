
################################################################################
# Dep: LibOpenCM3

LIB_OPENCM3_NAME = opencm3_stm32f4
LIB_OPENCM3_DIR  = $(PROJECT_ROOT)/deps/libopencm3
LIB_OPENCM3      = $(LIB_OPENCM3_DIR)/lib/lib$(LIB_OPENCM3_NAME).a

$(LIB_OPENCM3):
	$(MAKE) -C $(LIB_OPENCM3_DIR)

LD_LIB_NAMES += $(LIB_OPENCM3_NAME)
LD_LIB_PATHS += $(LIB_OPENCM3)

LD_FLAGS += -L$(LIB_OPENCM3_DIR)/lib
CC_FLAGS += -I$(LIB_OPENCM3_DIR)/include
