#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "kernel.h"
#include "vga.h"

static const char sc_lower[] = {
    0,0,
    '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',0,0,0,' '
};

static const char sc_upper[] = {
    0,0,
    '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?',0,0,0,' '
};

static int shift = 0;
static int caps  = 0;

void keyboard_handler() {
    uint8_t sc = inb(0x60);

    if (sc == 0xE0) { outb(PIC1_CMD, PIC_EOI); return; }

    if (sc == 0x2A || sc == 0x36) { shift = 1; outb(PIC1_CMD, PIC_EOI); return; }
    if (sc == 0xAA || sc == 0xB6) { shift = 0; outb(PIC1_CMD, PIC_EOI); return; }
    if (sc == 0x3A) { caps ^= 1;               outb(PIC1_CMD, PIC_EOI); return; }

    if (sc & 0x80) { outb(PIC1_CMD, PIC_EOI); return; }

    if (sc < sizeof(sc_lower)) {
        int upper = shift ^ caps;
        char c = upper ? sc_upper[sc] : sc_lower[sc];
        if (c) vga_putchar(c);
    }

    outb(PIC1_CMD, PIC_EOI);
}

#endif