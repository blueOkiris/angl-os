#pragma once

#include <stddef.h>
#include <stdint.h>

void terminal__init();
void terminal__set_color(uint8_t color);
void terminal__put_char(char c);
void terminal__put_string(const char *str);

const static struct {
    void (*init)();
    void (*set_color)(uint8_t color);
    void (*put_char)(char c);
    void (*put_string)(const char *c);
} terminal = {
    terminal__init,
    terminal__set_color,
    terminal__put_char,
    terminal__put_string
};
