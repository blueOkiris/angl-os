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
            White = 15
        };
        
        class Terminal {
            private:
                static Terminal _instance;
                static bool _hasInitialized;
                
                size_t _row, _col;
                TerminalColor _color;
                
                void init();
                void _putEntryAt(
                    const char &c, const TerminalColor &color,
                    const size_t &x, const size_t &y
                );
                void _updateCursor(const size_t &x, const size_t &y);
            
            public:
                static Terminal *instance();
            
                void setColor(const TerminalColor &color);
                void putChar(const char &c);
                void putInteger(const uint32_t &d);
                void putStr(const char *str);
        };
    }
}
