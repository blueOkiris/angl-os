#pragma once

namespace angl {
    namespace kernel {
        struct Page {
            uint32_t present : 1;
            uint32_t readWrite : 1;
            uint32_t userLevel : 1;
            uint32_t accessed : 1;
            uint32_t writtenTo : 1;
            uint32_t unused : 7;
            uint32_t frame : 20;
        } __attribute__((packed));

        struct PageTable {
            Page pages[1024];
        };

        struct PageDirectory {
            PageTable *tables[1024];
            uint32_t tablesPhysicalAddr[1024];
            uint32_t physicalAddress;
        };

        namespace memory {
            uint32_t allocate(
                uint32_t size, bool align, uint32_t *physAddr
            );
            void set(uint8_t *dest, uint8_t value, uint32_t len);
        }

        namespace frame {
            void set(uint32_t frameAddr);
            void clear(uint32_t frameAddr);
            uint32_t test(uint32_t frameAddr);
            uint32_t first();
            void allocate(Page *page, bool kernelLevel, bool writeable);
            void destroy(Page *page);
        }

        namespace paging {
            void init();
        }
    }
}
