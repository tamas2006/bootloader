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
    call keyboard_handler
    popa
    iret