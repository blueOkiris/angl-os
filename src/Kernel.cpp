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
    asm volatile ("int $3");
    asm volatile ("int $31");
    io::terminal::putStr("Done testing!\n");

    io::terminal::putStr("\nNow just running clock...\n");
    irq::enable(0);
    asm volatile ("int $32");
    //timer::init(1);

    while(true);
}
