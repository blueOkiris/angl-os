#include <stddef.h>
#include <stdint.h>
#include <Isr.hpp>
#include <IdtGdt.hpp>

using namespace angl;
using namespace kernel;

extern "C" void gdt_flush(uint32_t gdtPtr);
extern "C" void idt_flush(uint32_t idtPtr);

GdtEntry Gdt::_gdt[5];
DescPtr Gdt::_ptr;
IdtEntry Idt::_idt[256];
DescPtr Idt::_ptr;

void Gdt::init() {
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

void Idt::init() {
    _ptr.limit = (sizeof(IdtEntry) * 256) - 1;
    _ptr.base = reinterpret_cast<uint32_t>(&_idt);
    
    for(auto &idt : _idt) {
        idt = (IdtEntry) { 0, 0, 0, 0, 0 };
    }

    _setGate(0, reinterpret_cast<uint32_t>(isr0), 0x08, 0x8E);
    _setGate(1, reinterpret_cast<uint32_t>(isr1), 0x08, 0x8E);
    _setGate(2, reinterpret_cast<uint32_t>(isr2), 0x08, 0x8E);
    _setGate(3, reinterpret_cast<uint32_t>(isr3), 0x08, 0x8E);
    _setGate(4, reinterpret_cast<uint32_t>(isr4), 0x08, 0x8E);
    _setGate(5, reinterpret_cast<uint32_t>(isr5), 0x08, 0x8E);
    _setGate(6, reinterpret_cast<uint32_t>(isr6), 0x08, 0x8E);
    _setGate(7, reinterpret_cast<uint32_t>(isr7), 0x08, 0x8E);
    _setGate(8, reinterpret_cast<uint32_t>(isr8), 0x08, 0x8E);
    _setGate(9, reinterpret_cast<uint32_t>(isr9), 0x08, 0x8E);
    _setGate(10, reinterpret_cast<uint32_t>(isr10), 0x08, 0x8E);
    _setGate(11, reinterpret_cast<uint32_t>(isr11), 0x08, 0x8E);
    _setGate(12, reinterpret_cast<uint32_t>(isr12), 0x08, 0x8E);
    _setGate(13, reinterpret_cast<uint32_t>(isr13), 0x08, 0x8E);
    _setGate(14, reinterpret_cast<uint32_t>(isr14), 0x08, 0x8E);
    _setGate(15, reinterpret_cast<uint32_t>(isr15), 0x08, 0x8E);
    _setGate(16, reinterpret_cast<uint32_t>(isr16), 0x08, 0x8E);
    _setGate(17, reinterpret_cast<uint32_t>(isr17), 0x08, 0x8E);
    _setGate(18, reinterpret_cast<uint32_t>(isr18), 0x08, 0x8E);
    _setGate(19, reinterpret_cast<uint32_t>(isr19), 0x08, 0x8E);
    _setGate(20, reinterpret_cast<uint32_t>(isr20), 0x08, 0x8E);
    _setGate(21, reinterpret_cast<uint32_t>(isr21), 0x08, 0x8E);
    _setGate(22, reinterpret_cast<uint32_t>(isr22), 0x08, 0x8E);
    _setGate(23, reinterpret_cast<uint32_t>(isr23), 0x08, 0x8E);
    _setGate(24, reinterpret_cast<uint32_t>(isr24), 0x08, 0x8E);
    _setGate(25, reinterpret_cast<uint32_t>(isr25), 0x08, 0x8E);
    _setGate(26, reinterpret_cast<uint32_t>(isr26), 0x08, 0x8E);
    _setGate(27, reinterpret_cast<uint32_t>(isr27), 0x08, 0x8E);
    _setGate(28, reinterpret_cast<uint32_t>(isr28), 0x08, 0x8E);
    _setGate(29, reinterpret_cast<uint32_t>(isr29), 0x08, 0x8E);
    _setGate(30, reinterpret_cast<uint32_t>(isr30), 0x08, 0x8E);
    _setGate(31, reinterpret_cast<uint32_t>(isr31), 0x08, 0x8E);

    idt_flush(reinterpret_cast<uint32_t>(&_ptr));
}

void Idt::_setGate(int num, uint32_t base, uint16_t selector, uint8_t flags) {
    _idt[num].baseLow = base & 0xFFFF;
    _idt[num].baseHigh = (base >> 16) & 0xFFFF;

    _idt[num].selector = selector;
    _idt[num].alwaysZero = 0;

    // Uncomment the 0x60 when in user mode
    _idt[num].flags = flags; // | 0x60;
}