#include <stddef.h>
#include <stdint.h>
#include <io/Terminal.hpp>
#include <io/Ports.hpp>
#include <device/Timer.hpp>
#include <kernel/Kernel.hpp>
#include <kernel/IsrIrq.hpp>

using namespace angl;
using namespace kernel;

InterruptController InterruptController::_instance;

void InterruptController::isrHandler(RegisterSet regs) {
    
    auto instance = InterruptController::instance();
    if(instance->handlers[regs.interruptNumber] == NULL) {
        auto terminal = io::Terminal::instance();
        terminal->putStr("  Received interrupt: ");
        terminal->putInteger(regs.interruptNumber);
        terminal->putChar('\n');
    } else {
        instance->handlers[regs.interruptNumber](regs);
    }
}

void InterruptController::irqHandler(RegisterSet regs) {
    if(regs.interruptNumber >= 40) {
        io::port::write(0xA0, 0x20); // Send reset signal to slave
    }
    io::port::write(0x20, 0x20); // Send reset signal to master
    
    auto instance = InterruptController::instance();
    if(instance->handlers[regs.interruptNumber] == NULL) {
        auto terminal = io::Terminal::instance();
        terminal->putStr("IRQ received, but no handler defined.\n");
    } else {
        instance->handlers[regs.interruptNumber](regs);
    }
}

InterruptController *InterruptController::instance() {
    return &_instance;
}

void InterruptController::enableIrq(const uint32_t &num) {
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

void InterruptController::disableIrq(const uint32_t &num) {
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
