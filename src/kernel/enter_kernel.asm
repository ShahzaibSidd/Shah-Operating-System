[bits 32]
global _start
extern main

_start:
    push ebx ; Push Multiboot info pointer as second argument
    push eax ; Push Multiboot magic as first argument
    call main
    jmp $