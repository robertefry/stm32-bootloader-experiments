
#define LD_BASE(prefix) LD_BASE_(prefix)
#define LD_BASE_(prefix) (prefix##_BASE)

#define LD_SIZE(prefix) LD_SIZE_(prefix)
#define LD_SIZE_(prefix) (prefix##_SIZE)


#define BOOTLOADER_BASE     0x08000000
#define BOOTLOADER_SIZE     0x00008000 //  32 KiB

#define FIRMWARE_BASE       0x08008000
#define FIRMWARE_SIZE       0x00078000 // 480 KiB
