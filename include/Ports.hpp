#pragma once

#include <stddef.h>
#include <stdint.h>

namespace angl {
    namespace io {
        namespace port {
            void write(uint32_t port, uint8_t data);
            uint8_t read(uint32_t port);
        }
    }
}
