ENTRY(_start)

SECTIONS
{
    /* The kernel virtual address starts at 3GB + 1MB */
    . = 0xC0100000;

    /* Code section: linked at 0xC0100000, loaded at physical 0x100000 */
    .text : AT(ADDR(.text) - 0xC0000000)
    {
        *(.text)
    }

    /* Read-only data section */
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
    {
        *(.rodata)
    }

    /* Initialized data section */
    .data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data)
    }

    /* Uninitialized data section (BSS) and stack */
    .bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss)
    }

    /* The symbol used by PMM to know where kernel ends (virtual address) */
    kernel_end = ALIGN(4K);
}
