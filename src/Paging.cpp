#include <stddef.h>
#include <stdint.h>
#include <IsrIrq.hpp>
#include <Kernel.hpp>

using namespace angl;
using namespace kernel;

extern "C" void load_page_directory(uint32_t *);
extern "C" void enable_paging();

void Kernel::_enablePaging() {
    uint32_t pageDir[1024] __attribute__((aligned(4096)));
    for(int i = 0; i < 1024; i++) {
        // Set everything to not present
        pageDir[i] = 0x00000002; // Supervisor | Writeable | Not Present
    }

    uint32_t primaryPageTable[1024] __attribute__((aligned(4096)));
    for(int i = 0; i < 128; i++) {
        primaryPageTable[i] = (i * 0x1000) | 3; // Su | Read/Write | Present
    }
    for(int i = 128; i < 1024; i++) {
        primaryPageTable[i] = (i * 0x1000) | 2; // Su | Read/Write | Not Present
    }
    primaryPageTable[0xB8000 / 0x1000] = 0xB8000 | 3; // Video present

    uint32_t kernelPageTable[1024] __attribute__((aligned(4096)));
    for(int i = 0; i < 1024; i++) {
        kernelPageTable[i] = (0x400000 + (i * 0x1000)) | 3; // Su | R/W | Pres
    }

    uint32_t fileSystemPageTable[1024] __attribute__((aligned(4096)));
    for(int i = 0; i < 1024; i++) {
        fileSystemPageTable[i] = (0x8000000 + (i * 0x1000)) | 3;
    }

    pageDir[0] = reinterpret_cast<uint32_t>(primaryPageTable) | 3;
    pageDir[1] = reinterpret_cast<uint32_t>(kernelPageTable) | 3;
    pageDir[2] = reinterpret_cast<uint32_t>(fileSystemPageTable) | 3;

    // Enable paging
    load_page_directory(pageDir);
    enable_paging();
}
