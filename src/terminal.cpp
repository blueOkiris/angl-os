#include <stddef.h>
#include <stdint.h>
#include <color.hpp>
#include <terminal.hpp>

using namespace angl;
using namespace terminal;

/*
 * Helper functions
 */
inline Color makeColor(Color fg_color, Color bg_color) {
    return static_cast<Color>(
        static_cast<uint8_t>(fg_color) | (static_cast<uint8_t>(bg_color) << 4)
    );
}

inline uint16_t makeVgaEntry(char c, Color color) {
    uint16_t char16 = c;
    uint16_t color16 = static_cast<uint8_t>(color);
    return char16 | (color16 << 8);
}

inline size_t strLen(const char *str) {
    size_t len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

/*
 * Main terminal functions
 */
void Terminal::_putEntryAt(char c, Color color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    _buffer[index] = makeVgaEntry(c, color);
}

Terminal::Terminal() : _row(0), _col(0),
        _color(makeColor(Color::LightGray, Color::Black)) {
    _buffer = (uint16_t *) VGA_MEMORY;
    for(size_t y = 0; y < VGA_HEIGHT; y++) {
        for(size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            _buffer[index] = makeVgaEntry(' ', _color);
        }
    }
}

void Terminal::setColor(Color color) {
    _color = color;
}

void Terminal::putChar(char c) {
    if(c == '\n') {
        _col = VGA_WIDTH - 1;
    } else {
        _putEntryAt(c, _color, _col, _row);
    }
    if(++_col == VGA_WIDTH) {
        _col = 0;
        if(++_row == VGA_HEIGHT) {
            _row = 0;
        }
    }
}

void Terminal::putStr(const char *str) {
    size_t len = strLen(str);
    for(size_t ind = 0; ind < len; ind++) {
        putChar(str[ind]);
    }
}
