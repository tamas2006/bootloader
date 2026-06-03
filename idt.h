#ifndef IDT_H
#define IDT_H
#include "kernel.h"

#define IDT_SIZE 256
#define PIC1     0x20
#define PIC2     0xA0
#define PIC1_CMD 0x20
#define PIC1_DAT 0x21
#define PIC2_CMD 0xA0
#define PIC2_DAT 0xA1
#define PIC_EOI  0x20

struct idt_gate {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_gate idt[IDT_SIZE];
static struct idt_ptr  idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo  = base & 0xFFFF;
    idt[num].base_hi  = (base >> 16) & 0xFFFF;
    idt[num].sel      = sel;
    idt[num].always0  = 0;
    idt[num].flags    = flags | 0x60;
}

void idt_install() {
    idtp.limit = (sizeof(struct idt_gate) * IDT_SIZE) - 1;
    idtp.base  = (uint32_t)&idt;
    __asm__ volatile("lidt %0"::"m"(idtp));
}

void pic_remap() {
    /* save masks */
    uint8_t m1 = inb(PIC1_DAT);
    uint8_t m2 = inb(PIC2_DAT);
    /* init sequence */
    outb(PIC1_CMD, 0x11); io_wait();
    outb(PIC2_CMD, 0x11); io_wait();
    /* vector offsets */
    outb(PIC1_DAT, 0x20); io_wait();  /* IRQ0-7  -> INT 32-39 */
    outb(PIC2_DAT, 0x28); io_wait();  /* IRQ8-15 -> INT 40-47 */
    /* cascade */
    outb(PIC1_DAT, 0x04); io_wait();
    outb(PIC2_DAT, 0x02); io_wait();
    /* 8086 mode */
    outb(PIC1_DAT, 0x01); io_wait();
    outb(PIC2_DAT, 0x01); io_wait();
    /* restore masks — unmask only IRQ1 (keyboard) */
    outb(PIC1_DAT, m1 & ~0x02);
    outb(PIC2_DAT, m2);
}

#endif