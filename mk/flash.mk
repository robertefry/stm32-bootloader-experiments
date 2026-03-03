
STFLASH := st-flash

ifndef FLASH_ORIGIN
  $(error "Variable FLASH_ORIGIN is not defined.")
endif

flash: $(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).bin
	$(STFLASH) write "$<" $(FLASH_ORIGIN)
