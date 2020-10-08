#include <stddef.h>
#include <stdint.h>
#include <Terminal.hpp>
#include <Isr.hpp>

using namespace angl;
using namespace kernel;

void Isr::handler(RegisterSet regs) {
    io::terminal::init();
    io::terminal::putStr("Received interrupt!\nInterrupt: 0x");
    io::terminal::putInteger(regs.interrupNumber);
    io::terminal::putChar('\n');
}
