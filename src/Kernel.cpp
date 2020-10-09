#include <IdtGdt.hpp>
#include <Terminal.hpp>
#include <Timer.hpp>
#include <Kernel.hpp>

using namespace angl;
using namespace kernel;

void angl::kernel::main() {
    gdt::init();
    idt::init();

    io::terminal::init();
    io::terminal::putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    io::terminal::putStr("\nTesting IDT...\n");
    
    asm volatile ("int $0");
    asm volatile ("int $31");
    io::terminal::putStr("Done testing.\n");

    io::terminal::putStr("\nTesting IRQs...\n");
    io::terminal::putStr("  RTC test:\n");
    timer::init(50);
    timer::start();
    uint32_t lastTicks = timer::ticks();
    while(lastTicks < 10) {
        if(lastTicks != timer::ticks()) {
            lastTicks = timer::ticks();
            io::terminal::putStr("    Ticks: ");
            io::terminal::putInteger(timer::ticks());
            io::terminal::putChar('\n');
        }
    }
    timer::stop();
    io::terminal::putStr("  Done.\n");
    io::terminal::putStr("Done testing.\n");

    while(true);
}
