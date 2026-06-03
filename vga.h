#ifndef VGA_H
#define VGA_H
#include "kernel.h"

#define VGA_BASE   ((volatile uint16_t*)0xB8000)
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

typedef enum {
    VGA_BLACK=0, VGA_BLUE, VGA_GREEN, VGA_CYAN,
    VGA_RED, VGA_PURPLE, VGA_BROWN, VGA_LGRAY,
    VGA_DGRAY, VGA_LBLUE, VGA_LGREEN, VGA_LCYAN,
    VGA_LRED, VGA_PINK, VGA_YELLOW, VGA_WHITE
} vga_color;

static inline uint16_t vga_entry(char c, uint8_t fg, uint8_t bg) {
    return (uint16_t)c | ((uint16_t)((bg<<4)|fg) << 8);
}

static int vga_row = 2;
static int vga_col = 0;
static uint8_t vga_fg = VGA_LGREEN;
static uint8_t vga_bg = VGA_BLACK;

void vga_clear() {
    int r, c;
    for (r = 0; r < VGA_HEIGHT; r++)
        for (c = 0; c < VGA_WIDTH; c++)
            VGA_BASE[r * VGA_WIDTH + c] = vga_entry(' ', VGA_LGRAY, VGA_BLACK);
}

void vga_putchar_at(int row, int col, char ch, uint8_t fg, uint8_t bg) {
    VGA_BASE[row * VGA_WIDTH + col] = vga_entry(ch, fg, bg);
}

void vga_fill_row(int row, char ch, uint8_t fg, uint8_t bg) {
    int c;
    for (c = 0; c < VGA_WIDTH; c++)
        vga_putchar_at(row, c, ch, fg, bg);
}

void vga_print_at(int row, int col, const char* s, uint8_t fg, uint8_t bg) {
    int i = 0;
    while (s[i]) { vga_putchar_at(row, col+i, s[i], fg, bg); i++; }
}

void vga_putchar(char c) {
    if (c == '\b') {
        if (vga_col > 0) { vga_col--; vga_putchar_at(vga_row, vga_col, ' ', vga_fg, vga_bg); }
        return;
    }
    if (c == '\n') {
        vga_col = 0;
        if (++vga_row >= VGA_HEIGHT-1) vga_row = 2;
        return;
    }
    vga_putchar_at(vga_row, vga_col, c, vga_fg, vga_bg);
    if (++vga_col >= VGA_WIDTH) { vga_col = 0; if (++vga_row >= VGA_HEIGHT-1) vga_row = 2; }
}

void vga_print(const char* s) {
    while (*s) vga_putchar(*s++);
}

#endif