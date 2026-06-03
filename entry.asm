[BITS 32]
[GLOBAL _start]
[GLOBAL keyboard_isr]
[EXTERN kernel_main]
[EXTERN keyboard_handler]

_start:
    call kernel_main
    cli
    hlt

keyboard_isr:
    pusha

    mov al, 0x20
    out 0x20, al

    popa
    
    