#include <stddef.h>
#include <stdint.h>
#include <IsrIrq.hpp>
#include <Terminal.hpp>
#include <Ports.hpp>
#include <Timer.hpp>

using namespace angl;

static uint32_t tick_g = 0;

void timer::handler(kernel::RegisterSet regs) {
    tick_g++;
    io::terminal::init();
    io::terminal::putStr("Tick: ");
    io::terminal::putInteger(tick_g);
    io::terminal::putChar('\n');
}

void timer::init(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    io::port::write(0x43, 0x36);

    uint8_t low = static_cast<uint8_t>(divisor & 0xFF);
    uint8_t high = static_cast<uint8_t>((divisor >> 8) & 0xFF);
    io::port::write(0x40, low);
    io::port::write(0x40, high);
    kernel::irq::enable(0);
}
