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
}

uint32_t timer::ticks() {
    return tick_g;
}

void timer::start() {
    kernel::irq::enable(0);
}

void timer::stop() {
    kernel::irq::disable(0);
}

void timer::init(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    io::port::write(0x43, 0x36);

    uint8_t low = static_cast<uint8_t>(divisor & 0xFF);
    uint8_t high = static_cast<uint8_t>((divisor >> 8) & 0xFF);
    io::port::write(0x40, low);
    io::port::write(0x40, high);
}
