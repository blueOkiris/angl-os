#include <stddef.h>
#include <stdint.h>
#include <Ports.hpp>

using namespace angl;
using namespace io;

extern "C" uint32_t read_port(uint32_t port);
extern "C" uint32_t write_port(uint32_t port, uint32_t data);

uint8_t Port::read(uint8_t port) {
    return static_cast<uint8_t>(read_port(static_cast<uint32_t>(port)));
}

void Port::write(uint8_t port, uint8_t data) {
    write_port(static_cast<uint8_t>(port), static_cast<uint8_t>(data));
}
