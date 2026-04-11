ENTRY(_start)

SECTIONS
{
    /* The kernel lives at 1MB (0x100000) */
    . = 1M;

    /* Code section */
    .text : ALIGN(4K)
    {
        *(.text)
    }

    /* Read-only data section (constants) */
    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Initialized data section */
    .data : ALIGN(4K)
    {
        *(.data)
    }

    /* Uninitialized data section (BSS) and stack */
    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /* The symbol used by PMM to know where kernel ends */
    kernel_end = ALIGN(4K);
}
