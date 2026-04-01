org 0x7C00 ; this is where the loaded boot sector is in memory, need to tell cpu to always start at this address

mov bx, hello_msg
call print_string

mov bx, goodbye_msg
call print_string

jmp $

; helper function to print a string, with string address in bx register beforehand
print_string:
    pusha ; push all registers to stack before messing with them
    mov ah, 0x0e ; top of ax to 0e for interrupt
    
    ; loop which controls printing each character
    print_loop:
        mov cl, [bx] ; move character into cl
        cmp cl, 0 ; compare character to null terminator

        je done ; if null terminator reached, exit loop

        ; printing current character to screen in the case of not null terminator
        mov al, cl 
        int 0x10
        add bx, 1
        jmp print_loop

    ; where to jump when finished printing, pop register stack and return to callee
    done:
        popa
        ret

; writing the wanted strings in memory
hello_msg:
    db "Hello, World! ", 0
goodbye_msg:
    db "Goodbye!", 0

; the padding for bootloader
times 510 - ($-$$) db 0
dw 0xAA55