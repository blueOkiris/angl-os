#include <stddef.h>
#include <stdint.h>
#include <gdt.hpp>

using namespace angl;
using namespace kernel;

extern "C" void flushGdt(GdtPtr ptr);

Gdt::Gdt() {
    // Set up pointer and limit
    _ptr.limit = (sizeof(GdtEntry) * 3 - 1);
    _ptr.base = reinterpret_cast<uint32_t>(&_gdtEntries);

    // Null descriptor
    _setGate(0, 0, 0, 0, 0);

    /*
     * Code segment
     * 
     * Base addr: 0
     * Limit: 4GB
     * Granularity: 4KB
     * 32-bit opcodes
     * Code segment descriptor
     */
    _setGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /*
     * Data segment
     * 
     * Same thing as code, but says it's a data segment
     */
    _setGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Flush old gdt and install new changes
    flushGdt(_ptr);
}

void Gdt::_setGate(
        int num, uint32_t base, uint32_t limit,
        uint8_t access, uint8_t granularity) {
    // Descriptor base address
    _gdtEntries[num].baseLow = base & 0xFFFF;
    _gdtEntries[num].baseMiddle = (base >> 16) & 0xFF;
    _gdtEntries[num].baseHigh = (base >> 24) & 0xFF;

    // Set up the limits
    _gdtEntries[num].limitLow = limit & 0xFFFF;
    _gdtEntries[num].limitHigh = (limit >> 16) & 0x0F;

    // Set up granularity and access flags
    _gdtEntries[num].granularity = granularity & 0xF0;
    _gdtEntries[num].access = access;
}
