[bits 16]
get_mmap:
    pusha
    
    mov ax, 0
    mov es, ax

    mov di, 0x8000
    xor ebx, ebx
    
    mmap_loop:
        mov eax, 0xE820
        mov edx, 0x534D4150
        mov ecx, 24
        int 0x15
        
        jc mmap_done
        
        cmp eax, 0x534D4150
        jne mmap_done
        
        test ecx, ecx
        jz skip_entry
        
        add di, 24
    
    skip_entry:
        test ebx, ebx
        jnz mmap_loop
    
    mmap_done:
        popa
        ret
