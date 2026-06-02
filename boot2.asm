[BITS 16]
[ORG 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, msg_real
    call print16

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:protected_mode

print16:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print16
.done:
    ret

msg_real db "Booting TamasOS... switching to 32-bit!", 0x0D, 0x0A, 0

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0
gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

[BITS 32]
protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x200000
    mov esp, ebp

    mov edi, 0xB8000
    mov esi, msg_pm
    mov ah, 0x0A

.print_pm:
    lodsb
    or al, al
    jz .done_pm
    mov [edi], ax
    add edi, 2
    jmp .print_pm

.done_pm:
    cli
    hlt

msg_pm db "Welcome to TamasOS - 32-bit Protected Mode!", 0

times 510-($-$$) db 0
dw 0xAA55