#include <stddef.h>
#include <stdint.h>
#include <Terminal.hpp>
#include <Ports.hpp>
#include <Timer.hpp>
#include <IsrIrq.hpp>

using namespace angl;
using namespace kernel;

void isr::handler(RegisterSet regs) {
    io::terminal::init();
    io::terminal::putStr("Received interrupt!\nInterrupt: 0x");
    io::terminal::putInteger(regs.interrupNumber);
    io::terminal::putChar('\n');
}

void irq::handler(RegisterSet regs) {
    if(regs.interrupNumber >= 40) {
        io::port::write(0xA0, 0x20); // Send reset signal to slave
    }

    io::port::write(0x20, 0x20); // Send reset signal to master

    io::terminal::init();
    switch(regs.interrupNumber) {
        case 0:
            break;
        
        case 32:
            timer::handler(regs);
            break;

        default:
            io::terminal::putStr("IRQ received, but no handler defined.\n");
            break;
    }
}
