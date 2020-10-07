#include <stddef.h>
#include <stdint.h>
#include <paging.hpp>

using namespace angl;
using namespace kernel;

extern "C" void loadPageDirectory(unsigned int *);
extern "C" void enablePaging();

static uint32_t pageDirectory_g[1024] __attribute__((aligned(4096)));
static uint32_t firstPageTable_g[1024] __attribute__((aligned(4096)));

void paging::init() {
    for(int i = 0; i < 1024; i++) {
       /*
        * Set up the global page directory
        * 
        * This sets the following flags to the pages:
        *  - Supervisor: Only kernel-mode can access them
        *  - Write Enabled: It can be both read from and written to
        *  - Not Present: The page table is not present
        */
       pageDirectory_g[i] = 0x00000002;

       /*
        * Set up the first page directory
        * 
        * As the address is aligned, it will have 12 bits zeroed
        * Those bits used by the attrs 
        * Attributes:
        *  - Supervisor level
        *  - read/write
        *  - present
        */
       firstPageTable_g[i] = (i * 0x1000) | 3;
    }
    pageDirectory_g[0] = ((unsigned int) firstPageTable_g) | 3;
    loadPageDirectory(pageDirectory_g);
    enablePaging();
}