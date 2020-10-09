#pragma once

#include <stddef.h>
#include <stdint.h>
#include <IsrIrq.hpp>

namespace angl {
    namespace timer {
        void init(uint32_t frequency);
        uint32_t ticks();
        void start();
        void stop();
        void handler(kernel::RegisterSet regs);
    }
}