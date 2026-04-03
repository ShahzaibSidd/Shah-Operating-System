[bits 32]
VID_MEM equ 0xB8000
WHITE_ON_BLACK equ 0x0f

; helper function to print a string in protected mode, with string address in ebx register beforehand
print_string_pm:
    pusha ; push all registers to stack before messing with them
    mov eax, VID_MEM
    
    ; loop to handle printing each character
    print_loop_pm:
        mov cl, [ebx] ; move character into cl
        cmp cl, 0 ; compare character to null terminator

        je done_pm ; if null terminator reached, exit loop

        ; printing current character to screen in the case of not null terminator
        mov byte [eax], cl
        mov byte [eax + 1], 0x0f
        add eax, 2
        add ebx, 1
        jmp print_loop_pm

    ; where to jump when finished printing, pop register stack and return to callee
    done_pm:
        popa
        ret