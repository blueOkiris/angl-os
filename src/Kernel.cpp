#include <IdtGdt.hpp>
#include <Terminal.hpp>
#include <Kernel.hpp>

using namespace angl;
using namespace kernel;

void angl::kernel::main() {
    Gdt::init();
    Idt::init();

    io::Terminal::init();
    io::Terminal::putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");

    while(true);
}
