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

void dmg::CPU::SetRegisters(const RegisterInfo& register_info)
{
    PC.WriteWord(register_info.PC);
    SP.WriteWord(register_info.SP);
    A.Write(register_info.A);
    B.Write(register_info.B);
    C.Write(register_info.C);
    D.Write(register_info.D);
    E.Write(register_info.E);
    F.Write(register_info.F);
    H.Write(register_info.H);
    L.Write(register_info.L);
}

uint32_t dmg::CPU::Step()
{
    return control_unit.Step();
}

void dmg::CPU::Reset()
{
    PC.WriteWord(CARTRIDGE_HEADER);
    SP.WriteWord(0);
    A.Write(0);
    F.Write(0);
    B.Write(0);
    C.Write(0);
    D.Write(0);
    E.Write(0);
    H.Write(0);
    L.Write(0);
}

dmg::RegisterInfo dmg::CPU::GetRegisterInfo() const
{
    return RegisterInfo
    {
        .F = F.Read(),
        .A = A.Read(),
        .B = B.Read(),
        .C = C.Read(),
        .D = D.Read(),
        .E = E.Read(),
        .H = H.Read(),
        .L = L.Read(),
        .PC = PC.ReadWord(),
        .SP = SP.ReadWord(),
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
