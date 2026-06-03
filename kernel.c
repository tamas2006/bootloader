#include "kernel.h"
#include "vga.h"
#include "idt.h"
#include "keyboard.h"

extern void keyboard_isr();

void kernel_main() {
    vga_clear();

    vga_fill_row(0, ' ', VGA_WHITE, VGA_BLUE);
    vga_print_at(0, 27, "TamasOS v1.0", VGA_WHITE, VGA_BLUE);
    vga_print_at(0, 42, "x86 32-bit Kernel", VGA_YELLOW, VGA_BLUE);

    vga_fill_row(24, ' ', VGA_WHITE, VGA_BLUE);
    vga_print_at(24, 2, "TamasOS | Protected Mode | IRQ Keyboard Driver", VGA_WHITE, VGA_BLUE);

    vga_print_at(1, 2, ">>", VGA_YELLOW, VGA_BLACK);
    vga_row = 2; vga_col = 0;

    pic_remap();
    idt_set_gate(33, (uint32_t)keyboard_isr, 0x08, 0x8E);
    idt_install();

    __asm__ volatile("sti");
    while(1) __asm__ volatile("hlt");
}