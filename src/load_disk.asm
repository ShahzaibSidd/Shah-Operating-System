[bits 16]
load_disk:
    push dx

    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [BOOT_DRIVE]

    int 0x13

    jc disk_error

    pop dx
    cmp dh, al
    jne disk_error
    ret

disk_error:
    mov bx, ERROR_MSG
    call print_string
    call print_new_line
    jmp $

ERROR_MSG: db "ERROR: Failed to read disk!", 0