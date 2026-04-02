org 0x7C00 ; this is where the loaded boot sector is in memory, need to tell cpu to always start at this address

mov bx, hello_msg
call print_string

call print_new_line

mov bx, goodbye_msg
call print_string

call print_new_line

mov dx, 0xA3E9
call print_hex

jmp $

%include "print_string.asm"
%include "print_hex.asm"

; writing the wanted strings in memory
hello_msg:
    db "Hello, World! ", 0
goodbye_msg:
    db "Goodbye! ", 0

; the padding for bootloader
times 510 - ($-$$) db 0
dw 0xAA55