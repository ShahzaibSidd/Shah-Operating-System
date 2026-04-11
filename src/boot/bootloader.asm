[BITS 16]
[org 0x7C00] ; this is where the loaded boot sector is in memory, need to tell cpu to always start at this address
KERNEL_OFFSET equ 0x1000 

mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax

; keep track of 
mov [BOOT_DRIVE], dl

mov bp, 0x9000
mov sp, bp

mov bx, NORMAL_MODE_MSG
call print_string
call print_new_line

call load_kernel

call get_mmap

call load_pm

jmp $

%include "boot/get_mmap.asm"
%include "boot/print_string.asm"
%include "boot/print_hex.asm"
%include "boot/load_disk.asm"
%include "boot/pm/print_string_pm.asm"
%include "boot/pm/gdt.asm"
%include "boot/pm/load_pm.asm"

[bits 16]
load_kernel:
    mov bx, LOAD_KERNEL_MSG
    call print_string
    call print_new_line

    mov bx, KERNEL_OFFSET
    mov dh, 15
    mov dl, [BOOT_DRIVE]
    call load_disk

    ret


[bits 32]
begin_pm:
    mov ebx, PROTECTED_MODE_MSG
    call print_string_pm

    call KERNEL_OFFSET

    jmp $

; Variables
BOOT_DRIVE: db 0x00
NORMAL_MODE_MSG: db "Started in 16-bit mode", 0
PROTECTED_MODE_MSG: db "NOW IN 32-BIT PM!", 0
LOAD_KERNEL_MSG: db "Loading Kernel into memory...", 0

; the padding for bootloader
times 510 - ($-$$) db 0
dw 0xAA55