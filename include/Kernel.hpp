#pragma once

#include <IdtGdt.hpp>
#include <Terminal.hpp>

namespace angl {
    namespace kernel {
        class Kernel {
            private:
                Gdt _gdt;
                Idt _idt;
                io::Terminal *_terminal;
                
                static void _pageFaultHandler(const RegisterSet &regs);
                void _enablePaging();
                
                void _testIdt();
                void _testIrqThroughTimer();
                void _testPageFault();
                
            public:
                void run();
        };
    }
}
