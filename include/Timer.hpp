#pragma once

#include <stddef.h>
#include <stdint.h>
#include <IsrIrq.hpp>

namespace angl {
    namespace timer {
        void handler(kernel::RegisterSet regs);
        void init(uint32_t frequency);
    }
}