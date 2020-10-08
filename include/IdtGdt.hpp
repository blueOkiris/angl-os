#pragma once

#include <stddef.h>
#include <stdint.h>

namespace angl {
    namespace kernel {
        struct GdtEntry {
            uint16_t limitLow;
            uint16_t baseLow;
            uint8_t baseMid;
            uint8_t access;
            uint8_t granularity;
            uint8_t baseHigh;
        } __attribute__((packed));

        struct IdtEntry {
            uint16_t baseLow;
            uint16_t selector;
            uint8_t alwaysZero;
            uint8_t flags;
            uint16_t baseHigh;
        } __attribute__((packed));

        struct DescPtr {
            uint16_t limit;
            uint32_t base;
        } __attribute__((packed));

        namespace gdt {
            void init();
        }
        
        namespace idt {
            void init();
        }
    }
}