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

dmg::CPU::CPUInfo dmg::CPU::GetInfo() const
{
    return CPUInfo
    {
        .F = F.read(),
        .A = A.read(),
        .B = B.read(),
        .C = C.read(),
        .D = D.read(),
        .E = E.read(),
        .H = H.read(),
        .L = L.read(),
        /*
        .AF = AF.readWord(),
        .BC = BC.readWord(),
        .DE = DE.readWord(),
        .HL = HL.readWord(),
        */
        .PC = PC.readWord(),
        .SP = SP.readWord(),
    };
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
