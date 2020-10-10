#include <stddef.h>
#include <stdint.h>
#include <io/Ports.hpp>

using namespace angl;
using namespace io;

uint8_t port::read(uint16_t port) {
    uint8_t data;
    asm volatile("inb %1, %0" : "=a" (data) : "dN" (port));
    return data;
}

void port::write(uint16_t port, uint8_t data) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}
