[bits 16]
get_mmap:
    pusha

    mov di, 0x8000
    mov ebx, 0
    mmap_loop:
        mov eax, 0xE820
        mov ecx, 24
        mov edx, 0x534D4150
        int 0x15
        jc mmap_done
        add di, 24
        cmp ebx, 0
        jne mmap_loop
    
    mmap_done:
        popa
        ret
