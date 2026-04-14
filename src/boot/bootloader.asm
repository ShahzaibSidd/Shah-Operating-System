[BITS 16]
[org 0x7C00]
KERNEL_OFFSET equ 0x100000 
TEMP_KERNEL_LOAD_ADDR equ 0x10000

mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
mov [BOOT_DRIVE], dl
mov bp, 0x9000
mov sp, bp

call enable_a20
call load_kernel
call get_mmap
call load_pm
jmp $

enable_a20:
    pusha
    mov ax, 0x2401
    int 0x15
    popa
    ret

%include "boot/get_mmap.asm"
%include "boot/print_string.asm"
%include "boot/load_disk.asm"
%include "boot/pm/print_string_pm.asm"
%include "boot/pm/gdt.asm"
%include "boot/pm/load_pm.asm"

[bits 16]
load_kernel:
    mov ax, 0x1000
    mov es, ax
    mov bx, 0x0000
    mov dh, 50
    mov dl, [BOOT_DRIVE]
    call load_disk
    mov ax, 0
    mov es, ax
    ret

[bits 32]
begin_pm:
    ; Copy kernel to 0x100000
    mov esi, TEMP_KERNEL_LOAD_ADDR
    mov edi, KERNEL_OFFSET
    mov ecx, 50 * 512 / 4 ; count in dwords
    rep movsd

    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE: db 0x00
NORMAL_MODE_MSG: db "16-bit", 0
PROTECTED_MODE_MSG: db "32-bit", 0
LOAD_KERNEL_MSG: db "Loading...", 0

times 510 - ($-$$) db 0
dw 0xAA55