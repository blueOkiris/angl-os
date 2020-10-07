#include <stddef.h>
#include <stdint.h>
#include <color.h>
#include <terminal.h>

#define VGA_WIDTH   80
#define VGA_HEIGHT  24
#define VGA_MEMORY  0x000B8000

size_t terminal_row, terminal_col;
uint8_t terminal_color;
uint16_t *terminal_buffer;

uint8_t make_color(uint8_t fg_color, uint8_t bg_color) {
    return fg_color | (bg_color << 4);
}

uint16_t make_vga_entry(char c, uint8_t color) {
    uint16_t char16 = c;
    uint16_t color16 = color;
    return char16 | (color16 << 8);
}

size_t strlen(const char *str) {
    size_t len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

void terminal__put_entry_at(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vga_entry(c, color);
}

void terminal__init() {
    terminal_row = terminal_col = 0;
    terminal_color = make_color(COLOR_LIGHT_GRAY, COLOR_BLACK);
    terminal_buffer = (uint16_t *) VGA_MEMORY;
    for(size_t y = 0; y < VGA_HEIGHT; y++) {
        for(size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = make_vga_entry(' ', terminal_color);
        }
    }
}

void terminal__set_color(uint8_t color) {
    terminal_color = color;
}

void terminal__put_char(char c) {
    if(c == '\n') {
        terminal_col = VGA_WIDTH - 1;
    } else {
        terminal__put_entry_at(c, terminal_color, terminal_col, terminal_row);
    }
    if(++terminal_col == VGA_WIDTH) {
        terminal_col = 0;
        if(++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

void terminal__put_string(const char *str) {
    size_t len = strlen(str);
    for(size_t ind = 0; ind < len; ind++) {
        terminal__put_char(str[ind]);
    }
}
