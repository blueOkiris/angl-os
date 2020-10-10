#include <IdtGdt.hpp>
#include <Terminal.hpp>
#include <Timer.hpp>
#include <Paging.hpp>
#include <Kernel.hpp>

using namespace angl;
using namespace kernel;

void Kernel::_testIdt() {
    _terminal->putStr("\nTesting IDT...\n");
    asm volatile ("int $0");
    asm volatile ("int $31");
    _terminal->putStr("Done testing.\n");
}

void Kernel::_testIrqThroughTimer() {
    _terminal->putStr("\nTesting IRQs...\n");
    _terminal->putStr("  RTC test:\n");
    
    auto timer = device::Timer::instance();
    timer->setFrequency(50);
    timer->start();
    uint32_t lastTicks = timer->ticks();
    while(lastTicks < 10) {
        if(lastTicks != timer->ticks()) {
            lastTicks = timer->ticks();
            _terminal->putStr("    Ticks: ");
            _terminal->putInteger(timer->ticks());
            _terminal->putChar('\n');
        }
    }
    timer->stop();
    
    _terminal->putStr("  Done.\n");
    _terminal->putStr("Done testing.\n");
}

void Kernel::_testPageFault() {
    _terminal->putStr("Forcing a page fault...\n");
    uint32_t *ptr = (uint32_t *) 0xA0000000;
    uint32_t doPageFault = *ptr;
    _terminal->putInteger(doPageFault);
}

void Kernel::run() {
    _gdt.init();
    _idt.init();
    _terminal = io::Terminal::instance();
    
    //paging::init();
    
    _terminal->putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    _testIdt();
    _testIrqThroughTimer();
    //_testPageFault(); // Uncomment this function to cause a page fault

    while(true);
}
