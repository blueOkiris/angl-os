#include <stddef.h>
#include <stdint.h>
#include <Terminal.hpp>
#include <Isr.hpp>

using namespace angl;
using namespace kernel;

void Isr::handler(RegisterSet regs) {
    terminal::Terminal *term = terminal::Terminal::instance();
    term->putStr("Received interrupt!\nInterrupt: 0x");
    term->putInteger(regs.interrupNumber);
    term->putChar('\n');
}
