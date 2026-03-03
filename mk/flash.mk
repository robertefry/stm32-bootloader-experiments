
STFLASH := st-flash

ifndef FLASH_OFFSET
  $(error "Variable FLASH_OFFSET is not defined.")
endif

flash: $(BUILD_DIR)/$(BUILD)/$(PLATFORM)-$(BINARY).bin
	$(STFLASH) write "$<" $(FLASH_OFFSET)
