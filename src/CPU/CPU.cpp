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

void dmg::CPU::Decode()
{
    control_unit.Decode();
}

uint8_t dmg::CPU::Fetch()
{
    return m_bus.Read(PC++);
}

uint8_t dmg::CPU::ReadMemory(uint16_t address) const
{
    return m_bus.Read(address);
}

void dmg::CPU::WriteMemory(uint16_t address, uint8_t value)
{
    m_bus.Write(address, value);
}
