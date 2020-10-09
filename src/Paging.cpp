#include <stddef.h>
#include <stdint.h>
#include <IsrIrq.hpp>
#include <Paging.hpp>

using namespace angl;
using namespace kernel;

extern "C" void load_page_directory(uint32_t *);
extern "C" void enable_paging();

static uint32_t pageDir_g[1024] __attribute__((aligned(4096)));
static uint32_t primaryPageTable[1024] __attribute__((aligned(4096)));

void paging::init() {
    for(int i = 0; i < 1024; i++) {
        // Set everything to not present
        pageDir_g[i] = 0x00000002; // Supervisor | Writeable | Not Present

        // While doing that we can go ahead and fill in the first table too
        primaryPageTable[i] = (i * 0x1000) | 3; // Su | Read/Write | Present
    }
    pageDir_g[0] = reinterpret_cast<uint32_t>(primaryPageTable) | 3;

    irq::enable(14);

    // Enable paging
    load_page_directory(pageDir_g);
    enable_paging();
}
