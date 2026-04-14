[bits 32]
global _start
extern main

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22)

section .data
align 4096
boot_page_directory:
    ; 0th entry: identity mapping (for the first 4MB)
    ; bits: 7=1 (4MB page), 1=1 (R/W), 0=1 (Present)
    dd 0x00000083 
    times (KERNEL_PAGE_NUMBER - 1) dd 0
    ; KERNEL_PAGE_NUMBER entry: higher-half mapping (points to same first 4MB)
    dd 0x00000083 
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

section .text
_start:
    ; The linker thinks we are at 0xC0100000, but we are actually at 0x100000.
    ; So we need to calculate physical addresses for everything until we jump to high memory.
    
    ; Get physical address of boot_page_directory
    mov eax, (boot_page_directory - KERNEL_VIRTUAL_BASE)
    mov cr3, eax

    ; Enable PSE (4MB pages) in CR4
    mov eax, cr4
    or eax, 0x00000010
    mov cr4, eax

    ; Enable paging in CR0
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Now paging is ON. We can safely jump to the higher-half address.
    lea eax, [higher_half]
    jmp eax

higher_half:
    ; We are now executing at 0xC0100xxx!
    ; Set up a stack
    mov esp, stack_top
    
    call main
    jmp $

section .bss
align 16
stack_bottom:
    resb 16384 ; 16KB stack
stack_top:
