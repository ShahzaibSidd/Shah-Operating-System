gdt_start:
    gdt_null_entries:
        dd 0x00000000
        dd 0x00000000
    
    gdt_code:
        ; 00000000 1 1 0 0 1111 1 00 1 1010 00000000 0000000000000000 1111111111111111 
        ; = 0x00CF9A00 0000FFFF
        dd 0x0000FFFF
        dd 0x00CF9A00
    
    gdt_data:
        ; 00000000 1 1 0 0 1111 1 00 1 0010 00000000 0000000000000000 1111111111111111 
        ; = 0x00CF9200 0000FFFF
        dd 0x0000FFFF
        dd 0x00CF9200

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
