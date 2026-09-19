#include "CPU.hpp"

#include <utility>

dmg::CPU::CPU(MemoryBus& bus)
    : AF{A, F}
    , BC{ B, C}
    , DE{ D, E}
    , HL{H, L}
    , m_bus(bus)
{
}

uint8_t dmg::CPU::fetch()
{
    return m_bus.read(PC++);
}

uint8_t dmg::CPU::read_memory(uint16_t address) const
{
    return m_bus.read(address);
}

void dmg::CPU::write_memory(uint16_t address, uint8_t value)
{
    m_bus.write(address, value);
}
