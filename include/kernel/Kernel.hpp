#pragma once

#include <kernel/IdtGdt.hpp>
#include <kernel/IsrIrq.hpp>
#include <io/FileSystem.hpp>
#include <io/Terminal.hpp>

namespace angl {
    namespace kernel {
        class Kernel {
            private:
                Gdt _gdt;
                Idt _idt;
                io::FileSystem _fs;
                
                io::Terminal *_terminal;
                
                static void _pageFaultHandler(const RegisterSet &regs);
                void _enablePaging();
                
                void _testIdt();
                void _testIrqThroughTimer();
                void _testKeyboard();
                void _testFileSystem();
                void _testPageFault();
                
            public:
                void run();
        };
    }
}
