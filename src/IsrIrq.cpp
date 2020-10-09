#include <stddef.h>
#include <stdint.h>
#include <Terminal.hpp>
#include <Ports.hpp>
#include <Timer.hpp>
#include <Kernel.hpp>
#include <IsrIrq.hpp>

using namespace angl;
using namespace kernel;

void interruptcontroller::isrHandler(RegisterSet regs) {
    io::terminal::init();
    io::terminal::putStr("  Received interrupt: ");
    io::terminal::putInteger(regs.interruptNumber);
    io::terminal::putChar('\n');

    if(regs.interruptNumber == 14) {
        io::terminal::putStr("Ya done messed up. That's a page fault, buddy!");
        io::terminal::putStr("\nNot going to recover...");
        while(true);
    }
}

void interruptcontroller::disableIrq(uint32_t num) {
    uint32_t current;
    
    if(num < 8) { // If in master PIC 0->8
        current = io::port::read(0x21);
        current |= (0x01 << num);
        io::port::write(0x21, current);
    } else { // Otherwise is slave PIC 0->8
        current = io::port::read(0xA1);
        current |= (0x01 << (num - 8));
        io::port::write(0xA1, current);
    }
}

void interruptcontroller::enableIrq(uint32_t num) {
    uint32_t current;
    
    if(num < 8) { // If in master PIC 0->8
        current = io::port::read(0x21);
        current &= ~(0x01 << num);
        io::port::write(0x21, current);
    } else { // Otherwise is slave PIC 0->8
        current = io::port::read(0xA1);
        current &= ~(0x01 << (num - 8));
        io::port::write(0xA1, current);
    }
}

void interruptcontroller::irqHandler(RegisterSet regs) {
    io::terminal::init();

    if(regs.interruptNumber >= 40) {
        io::port::write(0xA0, 0x20); // Send reset signal to slave
    }
    io::port::write(0x20, 0x20); // Send reset signal to master

    switch(regs.interruptNumber) {    
        case 32:
            timer::handler(regs);
            break;
        
        case 46:
            io::terminal::putStr("Page fault!");
            while(true);
            break;

        default:
            io::terminal::putStr("IRQ received, but no handler defined.\n");
            break;
    }
}
