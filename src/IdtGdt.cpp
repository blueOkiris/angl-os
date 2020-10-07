#include <stddef.h>
#include <stdint.h>
#include <IdtGdt.hpp>

using namespace angl;
using namespace kernel;

extern "C" void gdt_flush(uint32_t);

Gdt::Gdt() {
    _ptr.limit = (sizeof(GdtEntry) * 5) - 1;
    _ptr.base = reinterpret_cast<uint32_t>(&_gdt);

    _setGate(0, 0, 0, 0, 0);
    _setGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
    _setGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
    _setGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment
    _setGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment

    gdt_flush(reinterpret_cast<uint32_t>(&_ptr));
}

void Gdt::_setGate(
        int num, uint32_t base, uint32_t limit,
        uint8_t access, uint8_t granularity) {
    _gdt[num].baseLow = base & 0xFFFF;
    _gdt[num].baseMid = (base >> 16) & 0xFF;
    _gdt[num].baseHigh = (base >> 24) & 0xFF;

    _gdt[num].limitLow = limit & 0xFFFF;
    _gdt[num].granularity = (limit >> 16) & 0x0F;

    _gdt[num].granularity |= granularity & 0xF0;
    _gdt[num].access = access;
}
