[BITS 16]
[org 0x7C00] ; this is where the loaded boot sector is in memory, need to tell cpu to always start at this address

mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax

mov [BOOT_DRIVE], dl

mov bp, 0x8000
mov sp, bp

mov bx, 0x9000
mov dh, 5
mov dl, [BOOT_DRIVE]

call load_disk

call load_pm

jmp $

[bits 32]
begin_pm:


%include "print_string.asm"
%include "print_hex.asm"
%include "load_disk.asm"
%include "gdt.asm"
%include "load_pm"

; Variables
BOOT_DRIVE: db 0x00

; the padding for bootloader
times 510 - ($-$$) db 0
dw 0xAA55

times 256 dw 0xAEAE
times 256 dw 0xFACE