#pragma once

#include <stddef.h>
#include <stdint.h>
#include <Color.hpp>

#define VGA_WIDTH   80
#define VGA_HEIGHT  24
#define VGA_MEMORY  0x000B8000

namespace angl {
    namespace terminal {
        class Terminal {
            private:
                size_t _row, _col;
                Color _color;
                uint16_t *_buffer;

                void _putEntryAt(char c, Color color, size_t x, size_t y);

            public:
                Terminal();
                void setColor(Color color);
                void putChar(char c);
                void putStr(const char *str);
        };
    }
}
