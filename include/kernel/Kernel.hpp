#pragma once

#include <kernel/IdtGdt.hpp>
#include <kernel/IsrIrq.hpp>
#include <io/Terminal.hpp>

namespace angl {
    namespace kernel {
        class Kernel {
            private:
                Gdt _gdt;
                Idt _idt;
                io::Terminal *_terminal;
                InterruptController *_interruptController;
                
                static void _pageFaultHandler(const RegisterSet &regs);
                void _enablePaging();
                
                void _testIdt();
                void _testIrqThroughTimer();
                void _testKeyboard();
                void _testPageFault();
                
            public:
                void run();
        };
    }
}
