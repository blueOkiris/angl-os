#include <stddef.h>
#include <stdint.h>
#include <Ports.hpp>

using namespace angl;
using namespace io;

extern "C" uint32_t read_port(uint32_t port);
extern "C" uint32_t write_port(uint32_t port, uint32_t data);

uint8_t port::read(uint32_t port) {
    return read_port(port);
}

void port::write(uint32_t port, uint8_t data) {
    write_port(port, data);
}
