#include <terminal.hpp>
#include <paging.hpp>
#include <kernel.hpp>

using namespace angl;
using namespace kernel;

void angl::kernel::main() {
    paging::init();

    terminal::Terminal console;
    console.putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    while(true);
}
