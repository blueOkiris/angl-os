#pragma once

#include <stddef.h>
#include <stdint.h>

namespace angl {
    namespace io {
        namespace port {
            void write(uint16_t port, uint8_t data);
            uint8_t read(uint16_t port);
        }
    }
}
