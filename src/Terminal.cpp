#include <stddef.h>
#include <stdint.h>
#include <Ports.hpp>
#include <Terminal.hpp>

using namespace angl;
using namespace io;

Terminal Terminal::_instance;
bool Terminal::_hasInitialized = false;

/*
 * Helper functions
 */
inline TerminalColor makeColor(
        const TerminalColor &fg_color, const TerminalColor &bg_color) {
    return static_cast<TerminalColor>(
        static_cast<uint8_t>(fg_color) | (static_cast<uint8_t>(bg_color) << 4)
    );
}

inline uint16_t makeVgaEntry(const char &c, const TerminalColor &color) {
    const uint16_t char16 = c;
    const uint16_t color16 = static_cast<uint8_t>(color);
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

Terminal *Terminal::instance() {
    if(!_hasInitialized) {
        _instance.init();
        _hasInitialized = true;
    }
    
    return &_instance;
}

void Terminal::_putEntryAt(
        const char &c, const TerminalColor &color,
        const size_t &x, const size_t &y) {
    const size_t index = y * VGA_WIDTH + x;
    *(uint16_t *) (VGA_MEMORY + index * 2) = makeVgaEntry(c, color);
}

void Terminal::_updateCursor(const size_t &x, const size_t &y) {
   uint16_t cursorLocation = y * VGA_WIDTH + x;
   io::port::write(0x3D4, 14); // Tell VGA we are setting the high byte
   io::port::write(0x3D5, cursorLocation >> 8); // Send the high byte
   io::port::write(0x3D4, 15); // Tell VGA we are setting the low byte
   io::port::write(0x3D5, cursorLocation); // Send the low byte
}

void Terminal::init() {
    _row = _col = 0;
    _color = makeColor(TerminalColor::White, TerminalColor::Blue);
    for(size_t y = 0; y < VGA_HEIGHT; y++) {
        for(size_t x = 0; x < VGA_WIDTH; x++) {
            const auto index = y * VGA_WIDTH + x;
            *(uint16_t *) (VGA_MEMORY + index * 2) = makeVgaEntry(' ', _color);
        }
    }
}

void Terminal::setColor(const TerminalColor &color) {
    _color = color;
}

void Terminal::putChar(const char &c) {
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
    _updateCursor(_col, _row);
}

void Terminal::putInteger(const uint32_t &d) {
    if(d == 0) {
        putChar('0');
        return;
    }

    char digits[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32_t currVal = d;
    int digitsInd = 10;
    while(currVal > 0) {
        digits[digitsInd--] = '0' + (currVal % 10);
        currVal /= 10;
    }

    digitsInd = 0;
    while(digitsInd < 11) {
        if(digits[digitsInd] >= '0' && digits[digitsInd] <= '9') {
            putChar(digits[digitsInd]);
        }
        digitsInd++;
    }
}

void Terminal::putStr(const char *str) {
    const auto len = strLen(str);
    for(size_t ind = 0; ind < len; ind++) {
        putChar(str[ind]);
    }
}
