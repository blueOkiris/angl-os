#include <stddef.h>
#include <stdint.h>
#include <IsrIrq.hpp>
#include <Ports.hpp>
#include <IdtGdt.hpp>

using namespace angl;
using namespace kernel;

extern "C" void gdt_flush(uint32_t gdtPtr);
extern "C" void idt_flush(uint32_t idtPtr);

static GdtEntry gdt_g[5];
static IdtEntry idt_g[256];
static DescPtr gdtPtr_g, idtPtr_g;

inline void setGate(
        int num, uint32_t base, uint32_t limit,
        uint8_t access, uint8_t granularity) {
    gdt_g[num].baseLow = base & 0xFFFF;
    gdt_g[num].baseMid = (base >> 16) & 0xFF;
    gdt_g[num].baseHigh = (base >> 24) & 0xFF;

    gdt_g[num].limitLow = limit & 0xFFFF;
    gdt_g[num].granularity = (limit >> 16) & 0x0F;

    gdt_g[num].granularity |= granularity & 0xF0;
    gdt_g[num].access = access;
}

inline void setGate(int num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt_g[num].baseLow = base & 0xFFFF;
    idt_g[num].baseHigh = (base >> 16) & 0xFFFF;

    idt_g[num].selector = selector;
    idt_g[num].alwaysZero = 0;

    // Uncomment the 0x60 when in user mode
    idt_g[num].flags = flags; // | 0x60;
}

void gdt::init() {
    gdtPtr_g.limit = (sizeof(GdtEntry) * 5) - 1;
    gdtPtr_g.base = reinterpret_cast<uint32_t>(&gdt_g);

    setGate(0, 0, 0, 0, 0);
    setGate(1, 0, 0x0000FFFF, 0x9A, 0xCF); // Kernel code segment
    setGate(2, 0, 0x0000FFFF, 0x92, 0xCF); // Kernel data segment
    setGate(3, 0, 0x0000FFFF, 0xFA, 0xCF); // User code segment
    setGate(4, 0, 0x0000FFFF, 0xF2, 0xCF); // User data segment

    gdt_flush(reinterpret_cast<uint32_t>(&gdtPtr_g));
}

void idt::init() {
    idtPtr_g.limit = (sizeof(IdtEntry) * 256) - 1;
    idtPtr_g.base = reinterpret_cast<uint32_t>(&idt_g);
    
    for(auto &idt : idt_g) {
        idt = (IdtEntry) { 0, 0, 0, 0, 0 };
    }

    // Remap IRQ table
    io::port::write(0x20, 0x11);
    io::port::write(0xA0, 0x11);

    io::port::write(0x21, 0x20);
    io::port::write(0xA1, 0x28);
    io::port::write(0x21, 0x04);
    io::port::write(0xA1, 0x02);
    io::port::write(0x21, 0x01);
    io::port::write(0xA1, 0x01);
    
    io::port::write(0x21, 0xFB);
    io::port::write(0xA1, 0xFF);

    setGate(0, reinterpret_cast<uint32_t>(isr0), 0x08, 0x8E);
    setGate(1, reinterpret_cast<uint32_t>(isr1), 0x08, 0x8E);
    setGate(2, reinterpret_cast<uint32_t>(isr2), 0x08, 0x8E);
    setGate(3, reinterpret_cast<uint32_t>(isr3), 0x08, 0x8E);
    setGate(4, reinterpret_cast<uint32_t>(isr4), 0x08, 0x8E);
    setGate(5, reinterpret_cast<uint32_t>(isr5), 0x08, 0x8E);
    setGate(6, reinterpret_cast<uint32_t>(isr6), 0x08, 0x8E);
    setGate(7, reinterpret_cast<uint32_t>(isr7), 0x08, 0x8E);
    setGate(8, reinterpret_cast<uint32_t>(isr8), 0x08, 0x8E);
    setGate(9, reinterpret_cast<uint32_t>(isr9), 0x08, 0x8E);
    setGate(10, reinterpret_cast<uint32_t>(isr10), 0x08, 0x8E);
    setGate(11, reinterpret_cast<uint32_t>(isr11), 0x08, 0x8E);
    setGate(12, reinterpret_cast<uint32_t>(isr12), 0x08, 0x8E);
    setGate(13, reinterpret_cast<uint32_t>(isr13), 0x08, 0x8E);
    setGate(14, reinterpret_cast<uint32_t>(isr14), 0x08, 0x8E);
    setGate(15, reinterpret_cast<uint32_t>(isr15), 0x08, 0x8E);
    setGate(16, reinterpret_cast<uint32_t>(isr16), 0x08, 0x8E);
    setGate(17, reinterpret_cast<uint32_t>(isr17), 0x08, 0x8E);
    setGate(18, reinterpret_cast<uint32_t>(isr18), 0x08, 0x8E);
    setGate(19, reinterpret_cast<uint32_t>(isr19), 0x08, 0x8E);
    setGate(20, reinterpret_cast<uint32_t>(isr20), 0x08, 0x8E);
    setGate(21, reinterpret_cast<uint32_t>(isr21), 0x08, 0x8E);
    setGate(22, reinterpret_cast<uint32_t>(isr22), 0x08, 0x8E);
    setGate(23, reinterpret_cast<uint32_t>(isr23), 0x08, 0x8E);
    setGate(24, reinterpret_cast<uint32_t>(isr24), 0x08, 0x8E);
    setGate(25, reinterpret_cast<uint32_t>(isr25), 0x08, 0x8E);
    setGate(26, reinterpret_cast<uint32_t>(isr26), 0x08, 0x8E);
    setGate(27, reinterpret_cast<uint32_t>(isr27), 0x08, 0x8E);
    setGate(28, reinterpret_cast<uint32_t>(isr28), 0x08, 0x8E);
    setGate(29, reinterpret_cast<uint32_t>(isr29), 0x08, 0x8E);
    setGate(30, reinterpret_cast<uint32_t>(isr30), 0x08, 0x8E);
    setGate(31, reinterpret_cast<uint32_t>(isr31), 0x08, 0x8E);

    setGate(32, reinterpret_cast<uint32_t>(irq0), 0x08, 0x8E);
    setGate(33, reinterpret_cast<uint32_t>(irq1), 0x08, 0x8E);
    setGate(34, reinterpret_cast<uint32_t>(irq2), 0x08, 0x8E);
    setGate(35, reinterpret_cast<uint32_t>(irq3), 0x08, 0x8E);
    setGate(36, reinterpret_cast<uint32_t>(irq4), 0x08, 0x8E);
    setGate(37, reinterpret_cast<uint32_t>(irq5), 0x08, 0x8E);
    setGate(38, reinterpret_cast<uint32_t>(irq6), 0x08, 0x8E);
    setGate(39, reinterpret_cast<uint32_t>(irq7), 0x08, 0x8E);
    setGate(40, reinterpret_cast<uint32_t>(irq8), 0x08, 0x8E);
    setGate(41, reinterpret_cast<uint32_t>(irq9), 0x08, 0x8E);
    setGate(42, reinterpret_cast<uint32_t>(irq10), 0x08, 0x8E);
    setGate(43, reinterpret_cast<uint32_t>(irq11), 0x08, 0x8E);
    setGate(44, reinterpret_cast<uint32_t>(irq12), 0x08, 0x8E);
    setGate(45, reinterpret_cast<uint32_t>(irq13), 0x08, 0x8E);
    setGate(46, reinterpret_cast<uint32_t>(irq14), 0x08, 0x8E);
    setGate(47, reinterpret_cast<uint32_t>(irq15), 0x08, 0x8E);

    idt_flush(reinterpret_cast<uint32_t>(&idtPtr_g));
    asm volatile ("sti");
}
