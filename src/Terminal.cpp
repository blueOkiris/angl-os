#include <stddef.h>
#include <stdint.h>
#include <Terminal.hpp>

using namespace angl;
using namespace io;

static bool termHasBeeninit_g = false;

static size_t currRow_g, currCol_g;
static TerminalColor currColor_g;
static uint16_t *textBuffer_g;

/*
 * Helper functions
 */
inline TerminalColor makeColor(TerminalColor fg_color, TerminalColor bg_color) {
    return static_cast<TerminalColor>(
        static_cast<uint8_t>(fg_color) | (static_cast<uint8_t>(bg_color) << 4)
    );
}

inline uint16_t makeVgaEntry(char c, TerminalColor color) {
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

inline void putEntryAt(char c, TerminalColor color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    textBuffer_g[index] = makeVgaEntry(c, color);
}

/*
 * Main terminal functions
 */
void terminal::init() {
    if(termHasBeeninit_g) {
        return;
    }

    currRow_g = currCol_g = 0;
    currColor_g = makeColor(TerminalColor::LightGray, TerminalColor::Black);
    textBuffer_g = (uint16_t *) VGA_MEMORY;
    for(size_t y = 0; y < VGA_HEIGHT; y++) {
        for(size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            textBuffer_g[index] = makeVgaEntry(' ', currColor_g);
        }
    }
    termHasBeeninit_g = true;
}

void terminal::setColor(TerminalColor color) {
    currColor_g = color;
}

void terminal::putChar(char c) {
    if(c == '\n') {
        currCol_g = VGA_WIDTH - 1;
    } else {
        putEntryAt(c, currColor_g, currCol_g, currRow_g);
    }
    if(++currCol_g == VGA_WIDTH) {
        currCol_g = 0;
        if(++currRow_g == VGA_HEIGHT) {
            currRow_g = 0;
        }
    }
}

void terminal::putInteger(uint32_t d) {
    uint32_t currVal = d;
    while(currVal > 0) {
        putChar('0' + (currVal % 10));
        currVal /= 10;
    }
}

void terminal::putStr(const char *str) {
    size_t len = strLen(str);
    for(size_t ind = 0; ind < len; ind++) {
        putChar(str[ind]);
    }
}
