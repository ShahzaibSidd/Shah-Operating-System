[bits 16]
; helper function to print a string, with string address in bx register beforehand
print_string:
    pusha ; push all registers to stack before messing with them
    mov ah, 0x0E ; top of ax to 0E for interrupt
    
    ; loop to handle printing each character
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

print_new_line:
    pusha
    mov ah, 0x0E
    
    mov al, 0x0A
    int 0x10
    
    mov al, 0x0D
    int 0x10
    
    popa
    ret