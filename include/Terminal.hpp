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

        namespace terminal {
            void init();

            void setColor(TerminalColor color);
            void putChar(char c);
            void putInteger(uint32_t d);
            void putStr(const char *str);
        }
    }
}
