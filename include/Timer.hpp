#pragma once

#include <stddef.h>
#include <stdint.h>
#include <IsrIrq.hpp>

namespace angl {
    namespace device {        
        class Timer {
            private:
                static Timer _instance;
                uint32_t _ticks;
                
                static void _handler(const kernel::RegisterSet &regs);
                Timer();
                
            public:
                static Timer *instance();
                
                void setFrequency(const uint32_t &frequency);
                void start();
                void stop();
                void tick();
                
                uint32_t ticks();
        };
    }
}