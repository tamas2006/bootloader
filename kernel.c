#define VGA ((volatile char*)0xB8000)
#define WIDTH 80
#define HEIGHT 25

void put_char(int row, int col, char c, char color) {
    VGA[(row * WIDTH + col) * 2] = c;
    VGA[(row * WIDTH + col) * 2 + 1] = color;
}

void print_at(int row, int col, const char* s, char color) {
    int i = 0;
    while (s[i]) { put_char(row, col+i, s[i], color); i++; }
}

void fill_row(int row, char c, char color) {
    int i;
    for (i = 0; i < WIDTH; i++) put_char(row, i, c, color);
}

void clear_screen() {
    int i, j;
    for (i = 0; i < HEIGHT; i++)
        for (j = 0; j < WIDTH; j++)
            put_char(i, j, ' ', 0x07);
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile("inb %1,%0":"=a"(ret):"Nd"(port));
    return ret;
}

void print_hex(int row, int col, unsigned char val) {
    char hex[] = "0123456789ABCDEF";
    put_char(row, col,   hex[(val >> 4) & 0xF], 0x0E);
    put_char(row, col+1, hex[val & 0xF], 0x0E);
}

void kernel_main() {
    clear_screen();
    fill_row(0, ' ', 0x1F);
    print_at(0, 20, "TamasOS - Keyboard Debug Mode", 0x1F);
    print_at(2, 2, "Status port 0x64:", 0x07);
    print_at(3, 2, "Data   port 0x60:", 0x07);
    print_at(5, 2, "Press keys - scancodes appear here:", 0x0A);

    int col = 2;
    int row = 6;
    unsigned char last = 0;

    while (1) {
        unsigned char status = inb(0x64);
        unsigned char data   = inb(0x60);

        print_hex(2, 22, status);
        print_hex(3, 22, data);

        if ((status & 0x01) && data != last) {
            print_hex(row, col, data);
            col += 3;
            if (col > 75) { col = 2; row++; }
            last = data;
        }
    }
}
