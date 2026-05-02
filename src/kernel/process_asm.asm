[bits 32]

section .text

global switch_to_user_mode

; void switch_to_user_mode(registers_t* regs, uint32_t page_directory)
switch_to_user_mode:
    ; save the arguments while we still have the old cr3
    mov eax, [esp + 4] ; regs pointer (on heap)
    mov ecx, [esp + 8] ; page directory (physical)

    ; Extract values from registers_t while heap is still mapped
    ; push them onto the current kernel stack
    ; iret expects: [esp] = EIP, [esp+4] = CS, [esp+8] = EFLAGS, [esp+12] = ESP, [esp+16] = SS
    
    mov ebx, [eax + 44] ; ss
    push ebx
    mov ebx, [eax + 24] ; user esp
    push ebx
    mov ebx, [eax + 36] ; eflags
    push ebx
    mov ebx, [eax + 40] ; cs
    push ebx
    mov ebx, [eax + 32] ; eip
    push ebx

    ; Also save other general purpose registers needed to restore later
    push dword [eax + 28] ; ebp
    push dword [eax + 20] ; edi
    push dword [eax + 16] ; esi
    push dword [eax + 12] ; edx
    push dword [eax + 8]  ; ecx
    push dword [eax + 4]  ; ebx
    push dword [eax + 0]  ; eax (saved value)

    ; Now switch to the process's page directory
    ; The current kernel stack is mapped in both PDs, so esp remains valid
    mov cr3, ecx

    ; Set data segment registers to user data (0x23)
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Load general purpose registers from the stack
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp

    ; Go to Ring 3
    ; Stack now contains: EIP, CS, EFLAGS, ESP, SS
    iret
