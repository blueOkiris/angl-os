#include <IdtGdt.hpp>
#include <Terminal.hpp>
#include <Timer.hpp>
#include <Paging.hpp>
#include <Kernel.hpp>

using namespace angl;
using namespace kernel;

inline void testIdt() {
    auto terminal = io::Terminal::instance();
    terminal->putStr("\nTesting IDT...\n");
    asm volatile ("int $0");
    asm volatile ("int $31");
    terminal->putStr("Done testing.\n");
}

inline void testIrqThroughTimer() {
    auto timer = device::Timer::instance();
    auto terminal = io::Terminal::instance();
    
    terminal->putStr("\nTesting IRQs...\n");
    terminal->putStr("  RTC test:\n");
    
    timer->setFrequency(50);
    timer->start();
    uint32_t lastTicks = timer->ticks();
    while(lastTicks < 10) {
        if(lastTicks != timer->ticks()) {
            lastTicks = timer->ticks();
            terminal->putStr("    Ticks: ");
            terminal->putInteger(timer->ticks());
            terminal->putChar('\n');
        }
    }
    timer->stop();
    terminal->putStr("  Done.\n");
    terminal->putStr("Done testing.\n");
}

inline void testPageFault() {
    auto terminal = io::Terminal::instance();
    terminal->putStr("Forcing a page fault...\n");
    uint32_t *ptr = (uint32_t *) 0xA0000000;
    uint32_t doPageFault = *ptr;
    terminal->putInteger(doPageFault);
}

void angl::kernel::main() {
    Gdt gdt;
    gdt.init();
    Idt idt;
    idt.init();
    //paging::init();
    
    auto terminal = io::Terminal::instance();
    terminal->putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    testIdt();
    testIrqThroughTimer();
    //testPageFault(); // Uncomment this function to cause a page fault

    while(true);
}
