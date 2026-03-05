
/* Define memory regions. */
MEMORY
{
    block0_flash (rx)  : ORIGIN = 0x08008000, LENGTH = 480K
    block1_sram0 (rwx) : ORIGIN = 0x20000000, LENGTH = 112K
    block1_sram1 (rwx) : ORIGIN = 0x2001C000, LENGTH =  16K
}

/* Define the entry point of the output file. */
ENTRY (reset_handler)

/* Enforce emmition of the vector table. */
EXTERN (vector_table)

/* Define sections. */
SECTIONS
{
    .text : {
        *(.vectors) /* Vector table */
        . = ALIGN(4);
        *(.text*)   /* Program code */
        . = ALIGN(4);
        *(.rodata*) /* Read-only data */
        . = ALIGN(4);
    } >block0_flash

    /* C++ Static constructors/destructors, also used for __attribute__
     * ((constructor)) and the likes */
    .preinit_array : {
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;
    } >block0_flash
    .init_array : {
        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;
    } >block0_flash
    .fini_array : {
        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;
    } >block0_flash

    /*
     * Another section used by C++ stuff, appears when using newlib with
     * 64bit (long long) printf support
     */
    .ARM.extab : {
        *(.ARM.extab*)
    } >block0_flash
    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >block0_flash

    . = ALIGN(4);
    _etext = .;

    /* block1_sram0, but not cleared on reset, eg boot/app comms */
    .noinit (NOLOAD) : {
        *(.noinit*)
    } >block1_sram0
    . = ALIGN(4);

    .data : {
        _data = .;
        *(.data*)       /* Read-write initialized data */
        *(.ramtext*)    /* "text" functions to run in block1_sram0 */
        . = ALIGN(4);
        _edata = .;
    } >block1_sram0 AT >block0_flash
    _data_loadaddr = LOADADDR(.data);

    .bss : {
        *(.bss*)        /* Read-write zero initialized data */
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >block1_sram0

    /*
     * The .eh_frame section appears to be used for C++ exception handling.
     * You may need to fix this if you're using C++.
     */
    /DISCARD/ : { *(.eh_frame) }

    . = ALIGN(4);
    end = .;
}

PROVIDE(_stack = ORIGIN(block1_sram0) + LENGTH(block1_sram0));
