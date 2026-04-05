[bits 16]
;helper function to print hex number passed into dx register
print_hex:
    pusha

    mov ax, dx
    and ax, 0xF000
    shr ax, 12
    cmp al, 10
    jl num1
    char1:
        add al, 55
        jmp end1
    num1:
        add al, 48
    end1:
        mov [HEX_OUT + 2], al
    
    mov ax, dx
    and ax, 0x0F00
    shr ax, 8
    cmp al, 10
    jl num2
    char2:
        add al, 55
        jmp end2
    num2:
        add al, 48
    end2:
        mov [HEX_OUT + 3], al

    mov ax, dx
    and ax, 0x00F0
    shr ax, 4
    cmp al, 10
    jl num3
    char3:
        add al, 55
        jmp end3
    num3:
        add al, 48
    end3:
        mov [HEX_OUT + 4], al

    mov ax, dx
    and ax, 0x000F
    cmp al, 10
    jl num4
    char4:
        add al, 55
        jmp end4
    num4:
        add al, 48
    end4:
        mov [HEX_OUT + 5], al

    mov bx, HEX_OUT
    call print_string
    
    popa
    ret

HEX_OUT: db "0x0000", 0