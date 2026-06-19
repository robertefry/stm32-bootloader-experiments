
#ifndef COMMON_MEMORYMAP_H
#define COMMON_MEMORYMAP_H

#ifndef STM32F4
    #error The memory map is only compatible with our STM32F446RET6 MPU.
#endif

#define BOOTLOADER_BASE     0x08000000U
#define BOOTLOADER_SIZE     0x00008000U //  32 KiB

#define FIRMWARE_BASE       0x08008000U
#define FIRMWARE_SIZE       0x00078000U // 480 KiB

#endif /* COMMON_MEMORYMAP_H */
