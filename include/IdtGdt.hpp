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

        struct GdtPtr {
            uint16_t limit;
            uint32_t base;
        } __attribute__((packed));

        class Gdt {
            private:
                GdtEntry _gdt[5];
                GdtPtr _ptr;

                void _setGate(
                    int num, uint32_t base, uint32_t limit,
                    uint8_t access, uint8_t granularity
                );
            
            public:
                Gdt();
        };
    }
}