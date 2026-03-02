
.PHONY: bootloader firmware

all: bootloader firmware

clean:
	$(MAKE) -C bootloader clean
	$(MAKE) -C firmware clean

erase:
	st-flash erase

flash: flash-bootloader flash-firmware

flash-bootloader:
	$(MAKE) -C bootloader flash

flash-firmware:
	$(MAKE) -C firmware flash

bootloader:
	$(MAKE) -C bootloader

firmware:
	$(MAKE) -C firmware
