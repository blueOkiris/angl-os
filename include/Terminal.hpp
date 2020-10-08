#pragma once

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH   80
#define VGA_HEIGHT  24
#define VGA_MEMORY  0x000B8000

namespace angl {
    namespace io {
        enum class TerminalColor {
            Black = 0,          Blue = 1,           Green = 2,
            Cyan = 3,           Red = 4,            Magenta = 5,
            Brown = 6,          LightGray = 7,      DarkGray = 8,
            LightBlue = 9,      LightGreen = 10,    LightCyan = 11,
            LightRed = 12,      LightMagenta = 13,  LightBrown = 14,
            Whit = 15
        };

        class Terminal {
            private:
                static bool _hasBeeninit;

                static size_t _row, _col;
                static TerminalColor _color;
                static uint16_t *_buffer;

                static void _putEntryAt(
                    char c, TerminalColor color, size_t x, size_t y
                );

            public:
                static void init();

                static void setColor(TerminalColor color);
                static void putChar(char c);
                static void putInteger(uint32_t d);
                static void putStr(const char *str);
        };
    }
}
