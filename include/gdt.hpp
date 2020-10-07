#pragma once

#include <stddef.h>
#include <stdint.h>

namespace angl {
    namespace kernel {
        /*
         * Defines a GDT entry. We say packed,
         * because it prevents the compiler from doing things
         * it thinks are best
         * Prevent compiler "optimization" by packing
         */
        struct GdtEntry {
            uint16_t limitLow;
            uint16_t baseLow;
            uint8_t baseMiddle;
            uint8_t access;
            uint8_t limitHigh : 4;
            uint8_t granularity : 4;
            uint8_t baseHigh;
        } __attribute__((packed));

        /*
         * Special pointer which includes the limit:
         *  - The max bytes taken up by the GDT, minus 1.
         * Again, this NEEDS to be packed
         */
        struct GdtPtr {
            uint16_t limit;
            uint32_t base;
        } __attribute__((packed));

        // The actual 3-entry GDT
        class Gdt {
            private:
                GdtEntry _gdtEntries[3];
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
