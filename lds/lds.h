
#define BASE(prefix) (prefix##_BASE)
#define SIZE(prefix) (prefix##_SIZE)

#define BOOTLOADER_BASE     0x08000000U
#define BOOTLOADER_SIZE     0x00008000U //  32 KiB

#define FIRMWARE_BASE       0x08008000U
#define FIRMWARE_SIZE       0x00078000U // 480 KiB
