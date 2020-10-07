#include <terminal.hpp>
#include <paging.hpp>
#include <gdt.hpp>
#include <kernel.hpp>

using namespace angl;
using namespace kernel;

void angl::kernel::main() {
    terminal::Terminal console;
    console.putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    while(true);
}
