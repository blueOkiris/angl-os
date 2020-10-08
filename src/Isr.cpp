#include <stddef.h>
#include <stdint.h>
#include <Terminal.hpp>
#include <Isr.hpp>

using namespace angl;
using namespace kernel;

void Isr::handler(RegisterSet regs) {
    io::Terminal::init();
    io::Terminal::putStr("Received interrupt!\nInterrupt: 0x");
    io::Terminal::putInteger(regs.interrupNumber);
    io::Terminal::putChar('\n');
}
