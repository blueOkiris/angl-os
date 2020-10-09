#include <stddef.h>
#include <stdint.h>
#include <Kernel.hpp>
#include <IsrIrq.hpp>
#include <Paging.hpp>

using namespace angl;
using namespace kernel;

static uint32_t placeAddr_g = 0;
static uint32_t *frames_g;
static uint32_t numFrames_g;

inline uint32_t indexFromBit(uint32_t currFrame) {
    return currFrame / (8 * 4);
}

inline uint32_t offsetFromBit(uint32_t currFrame) {
    return currFrame % (8 * 4);
}

void memory::set(uint8_t *dest, uint8_t value, uint32_t len) {
    uint8_t *temp = dest;
    for(; len != 0; len--) {
        *temp++ = value;
    }
}

uint32_t memory::allocate(uint32_t size, bool align, uint32_t *physAddr) {
    if(align && (placeAddr_g & 0x0FFFF000)) {
        placeAddr_g &= 0x0FFFF000;
        placeAddr_g += 0x1000;
    }

    if(physAddr != NULL) {
        *physAddr = placeAddr_g;
    }

    uint32_t temp = placeAddr_g;
    placeAddr_g += size;
    return temp;
}

void frame::set(uint32_t frameAddr) {
    const uint32_t currFrame = frameAddr / 0x1000;
    const uint32_t index = indexFromBit(currFrame);
    const uint32_t offset = offsetFromBit(currFrame);
    frames_g[index] |= 1 << offset;
}

void frame::clear(uint32_t frameAddr) {
    const uint32_t currFrame = frameAddr / 0x1000;
    const uint32_t index = indexFromBit(currFrame);
    const uint32_t offset = offsetFromBit(currFrame);
    frames_g[index] &= ~(1 << offset);
}

uint32_t frame::test(uint32_t frameAddr) {
    const uint32_t currFrame = frameAddr / 0x1000;
    const uint32_t index = indexFromBit(currFrame);
    const uint32_t offset = offsetFromBit(currFrame);
    return frames_g[index] & (1 << offset);
}

uint32_t frame::first() {
    for(uint32_t i = 0; i < indexFromBit(numFrames_g); i++) {
        if(frames_g[i] != 0xFFFFFFFF) { // Check if anything is free
            for(uint32_t j = 0; j < 32; j++) {
                uint32_t toTest = 1 << j;
                if(!(frames_g[i] & toTest)) {
                    return i * 4 * 8 + j;
                }
            }
        }
    }

    return 0xFFFFFFFF;
}

void frame::allocate(Page *page, bool kernel, bool writeable) {
    if(page->frame != 0) {
        return;
    } else {
        uint32_t index = first();
        if(index == (uint32_t) -1) {
            panic("No free frames_g!");
        }
        set(index * 0x1000);
        page->present = 1;
        page->readWrite = writeable ? 1 : 0;
        page->userLevel = kernel ? 0 : 1;
        page->frame = index;
    }
}

void frame::destroy(Page *page) {
    uint32_t currFrame;
    if(!(currFrame = page->frame)) {
        return;
    } else {
        clear(currFrame);
        page->frame = 0;
    }
}

inline Page *getPage(uint32_t addr, bool make, PageDirectory *dir) {
    addr /= 0x1000;
    uint32_t tableInd = addr / 1024;
    if(dir->tables[tableInd]) {
        return &dir->tables[tableInd]->pages[addr % 1024];
    } else if(make) {
        uint32_t temp;
        dir->tables[tableInd] = (PageTable *) memory::allocate(
            sizeof(PageTable), true, &temp
        );
        memory::set(
            reinterpret_cast<uint8_t *>(dir->tables[tableInd]), 0, 0x1000
        );
        dir->tablesPhysicalAddr[tableInd] = temp | 0x7; // Present, RW, user
        return &dir->tables[tableInd]->pages[addr % 1024];
    } else {
        return 0;
    }
}

inline void switchPageDir(PageDirectory *dir) {
    uint32_t cr0;

    asm volatile ("mov %0, %%cr3" : : "r" (&dir->tablesPhysicalAddr));
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
}

void paging::init() {
    uint32_t memEndPage = 0x01000000; // Assume physical memory is 16MB
    
    numFrames_g = memEndPage / 0x1000;
    frames_g = (uint32_t *) memory::allocate(
        indexFromBit(numFrames_g), false, NULL
    );
    memory::set(
        reinterpret_cast<uint8_t *>(frames_g), 0, indexFromBit(numFrames_g)
    );

    // Make a page directory
    PageDirectory *kernelPageDir_g = (PageDirectory *) memory::allocate(
        sizeof(PageDirectory), true, NULL
    );
    memory::set(
        reinterpret_cast<uint8_t *>(kernelPageDir_g), 0, sizeof(PageDirectory)
    );

    uint32_t i = 0;
    while(i < placeAddr_g) {
        frame::allocate(getPage(i, 1, kernelPageDir_g), 0, 0);
        i += 0x1000;
    }

    // Enable page faults
    irq::enable(14);
    switchPageDir(kernelPageDir_g);
}
