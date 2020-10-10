#include <stddef.h>
#include <stdint.h>
#include <IsrIrq.hpp>
#include <Terminal.hpp>
#include <Ports.hpp>
#include <Timer.hpp>

using namespace angl;
using namespace device;

Timer Timer::_instance = Timer();

void Timer::_handler(const kernel::RegisterSet &regs) {
    instance()->tick();
}

Timer *Timer::instance() {
    return &_instance;
}

Timer::Timer() {
    setFrequency(50);
}

void Timer::setFrequency(const uint32_t &frequency) {
    _ticks = 0;
    auto interruptController = kernel::InterruptController::instance();
    interruptController->handlers[32] = _handler;
    
    const uint32_t divisor = 1193180 / frequency;
    io::port::write(0x43, 0x36);

    const auto low = static_cast<uint8_t>(divisor & 0xFF);
    const auto high = static_cast<uint8_t>((divisor >> 8) & 0xFF);
    io::port::write(0x40, low);
    io::port::write(0x40, high);
}

void Timer::start() {
    auto interruptController = kernel::InterruptController::instance();
    interruptController->enableIrq(0);
}

void Timer::stop() {
    auto interruptController = kernel::InterruptController::instance();
    interruptController->disableIrq(0);
}

void Timer::tick() {
    _ticks++;
}

uint32_t Timer::ticks() {
    return _ticks;
}
