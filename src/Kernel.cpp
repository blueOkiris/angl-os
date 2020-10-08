#include <IdtGdt.hpp>
#include <Terminal.hpp>
#include <Kernel.hpp>

using namespace angl;
using namespace kernel;

void angl::kernel::main() {
    gdt::init();
    idt::init();

    io::terminal::init();
    io::terminal::putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    while(true);
}
