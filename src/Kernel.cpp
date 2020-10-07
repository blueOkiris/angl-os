#include <IdtGdt.hpp>
#include <Terminal.hpp>
#include <Kernel.hpp>

using namespace angl;
using namespace kernel;

void angl::kernel::main() {
    Gdt gdt;

    terminal::Terminal console;
    console.putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    while(true);
}
