#include "ControlUnit.hpp"

#include <corecrt_io.h>
#include <filesystem>

#include "Utils/Log.hpp"
#include "CPU.hpp"
#include "Utils/Math.hpp"

dmg::ControlUnit::ControlUnit(CPU& cpu)
    : m_cpu(cpu)
{
}

void dmg::ControlUnit::decode()
{
    const uint8_t opcode = m_cpu.read_memory(m_cpu.PC++);

   switch (opcode)
   {
       // 0x0 Column
   case 0x00: // NOP
       NOP();
       break;
   case 0x10: // STOP
       STOP();
       break;
   case 0x20: // JR NZ, s8
        JR_NZ();
       break;
   case 0x30: // JR NC, s8
        JR_NC();
   case 0x40: // LD B, B
       LD(m_cpu.B, m_cpu.B);
       break;
   case 0x50: // LD D, B
       LD(m_cpu.D, m_cpu.B);
       break;
   case 0x60: // LD H, B
       LD(m_cpu.H, m_cpu.B);
       break;
   case 0x70: // LD (HL), B
       LD(Address(m_cpu.HL.readWord()), m_cpu.B);
       break;
   case 0x80: // ADD A, B
       ADD(m_cpu.A, m_cpu.B);
       break;
   case 0x90: // SUB B
       SUB(m_cpu.B);
       break;
   case 0xA0: // ADD B
       AND(m_cpu.B);
       break;
   case 0xB0: // OR B
       OR(m_cpu.B);
       break;
   case 0xC0: // RET NZ
       RET_NZ();
       break;
   case 0xD0: // RET NC
       RET_NC();
       break;
   case 0xE0: // LD (a8), A
       LD_internal();
       break;
   case 0xF0: // LD A, (a8)
       LD_internal_a();
       break;

   // 0x1 Column
   case 0x01: // LD BC, d16
       LD(m_cpu.BC);
       break;
   case 0x11: // LD DE, d16
       LD(m_cpu.DE);
       break;
   case 0x21: // LD HL, d16
       LD(m_cpu.HL);
       break;
   case 0x31: // LD SP, d16
       LD(m_cpu.SP);
       break;
   case 0x41: // LD B, C
       LD(m_cpu.B, m_cpu.C);
       break;
   case 0x51: // LD D, C
       LD(m_cpu.D, m_cpu.C);
       break;
   case 0x61: // LD H, C
       LD(m_cpu.H, m_cpu.C);
       break;
   case 0x71: // LD (HL), C
       LD(Address(m_cpu.HL), m_cpu.C);
       break;
   case 0x81: // ADD A, C
       ADD(m_cpu.A, m_cpu.C);
       break;
   case 0x91: // SUB C
       SUB(m_cpu.C);
       break;
   case 0xA1: // AND C
       AND(m_cpu.C);
       break;
   case 0xB1: // OR C
       OR(m_cpu.C);
   case 0xC1: // POP BC
        POP(m_cpu.BC);
        break;
   case 0xD1: // POP DE
       POP(m_cpu.DE);
       break;
   case 0xE1: // POP HL
       POP(m_cpu.HL);
       break;
   case 0xF1: // POP AF
       POP(m_cpu.AF);
       break;

       // 0x2 Column
   case 0x02: // LD (BC), A
       LD(Address(m_cpu.BC), m_cpu.A);
       break;
   case 0x12: // LD (DE), A
       LD(Address(m_cpu.DE), m_cpu.A);
       break;
   case 0x22: // LD (HL+), A
       // ??? I am not sure if this is going to work as intented
       LD(Address(m_cpu.HL++), m_cpu.A);
       break;
   case 0x32: // LD (HL-), A
       // ??? I am not sure if this is going to work as intended
       LD(Address(m_cpu.HL--), m_cpu.A);
       break;
   case 0x42: // LD B, D
       LD(m_cpu.B, m_cpu.D);
       break;
   case 0x52: // LD D, D
       LD(m_cpu.D, m_cpu.D);
       break;
   case 0x62: // LD H, D
       LD(m_cpu.H, m_cpu.D);
       break;
   case 0x72: // LD (HL), D
       LD(Address(m_cpu.HL), m_cpu.D);
       break;
   case 0x82: // ADD A, D
       ADD(m_cpu.A, m_cpu.D);
       break;
   case 0x92: // SUB D
       SUB(m_cpu.D);
       break;
   case 0xA2: // AND D
       AND(m_cpu.D);
       break;
   case 0xB2: // OR D
       OR(m_cpu.D);
       break;
   case 0xC2: // JP NZ, a16
       JP_NZ();
       break;
   case 0xD2: // JP NC, a16
       JP_NC();
       break;
   case 0xE2: // LD (C), A
       LD_internal();
       break;
   case 0xF2: // LD A, (C)
       LD_internal_a();
       break;

       // Column 0x3
   case 0x03: // INC BC
       INC(m_cpu.BC);
       break;
   case 0x13: // INC DE
       INC(m_cpu.DE);
       break;
   case 0x23: // INC HL
       INC(m_cpu.HL);
       break;
   case 0x33: // INC SP
       INC(m_cpu.SP);
       break;
   case 0x43: // LD B, E
       LD(m_cpu.B, m_cpu.E);
       break;
   case 0x53: // LD D, E
       LD(m_cpu.D, m_cpu.E);
       break;
   case 0x63: // LD H, E
       LD(m_cpu.H, m_cpu.E);
       break;
   case 0x73: // LD (HL), E
       LD(Address(m_cpu.HL), m_cpu.E);
       break;
   case 0x83: // ADD A E
       ADD(m_cpu.A, m_cpu.E);
       break;
   case 0x93: // SUB E
       SUB(m_cpu.E);
       break;
   case 0xA3: // AND E
       AND(m_cpu.E);
       break;
   case 0xB3: // OR E
       OR(m_cpu.E);
       break;
   case 0xC3: // JP a16
       JP();
       break;
   case 0xD3: // Non-existent
       DMG_WARN("Instruction 0xD3 is used that is non-existent.\n");
       break;
   case 0xE3: // Non-existent
       DMG_WARN("Instruction 0xE3 is used that is non-existent.\n");
       break;
   case 0xF3:
       DI();
       break;

       // Column 0x4
   case 0x04: // INC B
       INC(m_cpu.B);
       break;
   case 0x14: // INC D
       INC(m_cpu.D);
       break;
   case 0x24: // INC H
       INC(m_cpu.H);
   case 0x34: // INC (HL)
       INC(Address(m_cpu.HL));
       break;
   case 0x44: // LD B, H
       LD(m_cpu.B, m_cpu.H);
       break;
   case 0x54: // LD D, H
       LD(m_cpu.D, m_cpu.H);
       break;
   case 0x64: // LD H, H
       LD(m_cpu.H, m_cpu.H);
       break;
   case 0x74: // LD (HL), H
       LD(Address(m_cpu.HL), m_cpu.H);
       break;
   case 0x84: // ADD A, H
       ADD(m_cpu.A, m_cpu.H);
       break;
   case 0x94: // SUB H
       SUB(m_cpu.H);
       break;
   case 0xA4: // AND H
       AND(m_cpu.H);
       break;
   case 0xB4: // OR H
       OR(m_cpu.H);
       break;
   case 0xC4: // CALL NZ, a16
       CALL_NZ();
       break;
   case 0xD4: // CALL NC, a16
       CALL_NC();
       break;

       // Column 0x5
   case 0x05: // DEC B
       DEC(m_cpu.B);
       break;
   case 0x15: // DEC D
       DEC(m_cpu.D);
       break;
   case 0x25: // DEC H
       DEC(m_cpu.H);
       break;
   case 0x35: // DEC (HL)
       DEC(Address(m_cpu.HL));
       break;
   case 0x45: // LD B, L
       LD(m_cpu.B, m_cpu.L);
       break;
   case 0x55: // LD D, L
       LD(m_cpu.D, m_cpu.L);
       break;
   case 0x65: // LD H, L
       LD(m_cpu.H, m_cpu.L);
       break;
   case 0x75: // LD (HL), L
       LD(Address(m_cpu.HL), m_cpu.L);
       break;
   case 0x85: // ADD A, L
       ADD(m_cpu.A, m_cpu.L);
       break;
   case 0x95: // SUB L
       SUB(m_cpu.L);
       break;
   case 0xA5: // AND L
       AND(m_cpu.L);
       break;
   case 0xB5: // OR L
       OR(m_cpu.L);
       break;
   case 0xC5: // PUSH BC
       PUSH(m_cpu.BC);
       break;
   case 0xD5: // PUSH DE
       PUSH(m_cpu.DE);
       break;
   case 0xE5: // PUSH HL
       PUSH(m_cpu.HL);
       break;
   case 0xF5: // PUSH AF
       PUSH(m_cpu.AF);
       break;

       // Column 0x6
   case 0x06: // LD B, d8
       LD(m_cpu.B, readD8());
       break;
   case 0x16: // LD D, d8
       LD(m_cpu.D, readD8());
       break;
   case 0x26: // LD H, d8
       LD(m_cpu.H, readD8());
       break;
   case 0x36: // LD (HL), d8
       LD(Address(m_cpu.HL), readD8());
       break;
   case 0x46: // LD B, (HL)
       LD(m_cpu.B, Address(m_cpu.HL));
       break;
   case 0x56: // LD D, (HL)
       LD(m_cpu.D, Address(m_cpu.HL));
       break;
   case 0x66: // LD H, (HL)
       LD(m_cpu.H, Address(m_cpu.HL));
       break;
   case 0x76: // HALT
       HALT();
       break;
   case 0x86: // ADD A, (HL)
       ADD(m_cpu.A, Address(m_cpu.HL));
       break;
   case 0x96: // SUB (HL)
       SUB(Address(m_cpu.HL));
       break;
   case 0xA6: // AND (HL)
       AND(Address(m_cpu.HL));
       break;
   case 0xB6: // OR (HL)
       OR(Address(m_cpu.HL));
       break;
   case 0xC6: // ADD A, d8
       ADD(m_cpu.A, readD8());
       break;
   case 0xD6: // SUB d8
       SUB(readD8());
       break;
   case 0xE6: // AND d8
       AND(readD8());
       break;
   case 0xF6: // OR d8
       OR(readD8());
       break;

       // Column 0x7
   case 0x07: // RLCA
       RLCA();
       break;
   case 0x17: // RLA
       RLA();
   case 0x27: // DAA
        DAA();
       break;
   case 0x37: // SCF
       SCF();
       break;
   case 0x47: // LD B, A
       LD(m_cpu.B, m_cpu.A);
       break;
   case 0x57: // LD D, A
       LD(m_cpu.D, m_cpu.A);
       break;
   case 0x67: // LD H, A
       LD(m_cpu.H, m_cpu.A);
       break;
   case 0x77: // LD (HL), A
       LD(Address(m_cpu.HL), m_cpu.A);
       break;
   case 0x87: // ADD A, A
       ADD(m_cpu.A, m_cpu.A);
       break;
   case 0x97: //  SUB A
       SUB(m_cpu.A);
       break;
   case 0xA7: // AND A
       AND(m_cpu.A);
       break;
   case 0xB7: // OR A
       OR(m_cpu.A);
       break;
   case 0xC7: // RST 0
       RST(0x00);
       break;
   case 0xD7: // RST 2
       RST(0x10);
       break;
   case 0xE7: // RST 4
       RST(0x20);
       break;
   case 0xF7: // RST 6
       RST(0x30);
       break;

       // Column 0x8
   case 0x08: // LD (a16), SP
       LD_SP();
       break;
   case 0x18: // JR s8
       JR();
       break;
   case 0x28: // JR Z, s8
       JR_Z(readS8());
       break;
   case 0x38: // JR C, s8
       JR_C(readS8());
       break;
   case 0x48: // LD C, B
       LD(m_cpu.C, m_cpu.B);
       break;
   case 0x58: // LD E, B
       LD(m_cpu.E, m_cpu.B);
       break;
   case 0x68: // LD L, B
       LD(m_cpu.L, m_cpu.B);
       break;
   case 0x78: // LD A, B
       LD(m_cpu.A, m_cpu.B);
       break;
   case 0x88: // ADC A, B
       ADC(m_cpu.A, m_cpu.B);
       break;
   case 0x98: // SBC A, B
       SBC(m_cpu.B);
       break;
   case 0xA8: // XOR B
       XOR(m_cpu.B);
       break;
   case 0xB8: // CP B
       CP(m_cpu.B);
       break;
   case 0xC8: // RET Z
        RET_Z();
       break;
   case 0xD8: // RET C
       RET_C();
       break;
   case 0xE8: // ADD SP, s8
       ADD(m_cpu.SP, readS8());
       break;
   case 0xF8: // LD HL, SP+s8
       LD(m_cpu.HL, m_cpu.SP.readWord() + readS8());
       break;

       // Column 0x09
   default:
       break;
       DMG_WARN("There is no matching instruction for this opcode: %d\n", opcode);
   }
}

void dmg::ControlUnit::NOP()
{
}

void dmg::ControlUnit::STOP()
{
    DMG_WARN("STOP function is not yet implemented.\n");
}

void dmg::ControlUnit::DI()
{
    DMG_WARN("DI instruction function is not yet implemented.\n");
}

void dmg::ControlUnit::HALT()
{
    DMG_WARN("HALT instruction function is not yet implemented.\n");
}

void dmg::ControlUnit::RLCA()
{
    uint8_t value = m_cpu.A.read();

    uint8_t first_bit = getBit(value, 7);

    m_cpu.F.set_z_flag(false);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_h_flag(false);
    m_cpu.F.set_c_flag((first_bit == 1));

    value = value << 1;

    setBit(value, 0, first_bit);
    m_cpu.A.write(value);
}

void dmg::ControlUnit::RLA()
{
    uint8_t value = m_cpu.A.read();

    uint8_t first_bit = getBit(value, 7);
    uint8_t old_carry_value = m_cpu.F.read_c_flag();

    m_cpu.F.set_z_flag(false);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_h_flag(false);
    m_cpu.F.set_c_flag((first_bit == 1));

    value = value << 1;

    setBit(value, 0, old_carry_value);
    m_cpu.A.write(value);
}

void dmg::ControlUnit::DAA()
{
    uint8_t offset = 0u;
    const uint8_t a_value = m_cpu.A.read();
    bool should_carry = false;

    if ((a_value & 0xF) > 0x09 || m_cpu.F.read_h_flag() == 1)
    {
        offset |= 0x06;
    }
    if (a_value > 0x99 || m_cpu.F.read_c_flag() == 1)
    {
        offset |= 0x60;
        should_carry = true;
    }

    uint8_t result = 0u;
    if (m_cpu.F.read_n_flag() == 0)
        result = a_value + offset;
    else
        result = a_value - offset;

    m_cpu.F.set_z_flag(result == 0);
    //m_cpu.F.set_n_flag();
    m_cpu.F.set_h_flag(false);
    m_cpu.F.set_c_flag(should_carry);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::SCF()
{
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_h_flag(false);
    m_cpu.F.set_c_flag(true);
}

void dmg::ControlUnit::JR_NZ()
{
    uint8_t s8 = readS8();
    if (m_cpu.F.read_z_flag() == 0)
    {
        m_cpu.PC.increase(s8);
    }
}

void dmg::ControlUnit::JR_NC()
{
    uint8_t s8 = readS8();
    if (m_cpu.F.read_c_flag() == 0)
    {
        m_cpu.PC.increase(s8);
    }
}

void dmg::ControlUnit::JR()
{
    m_cpu.PC.writeWord(m_cpu.PC.readWord() + readS8());
}

void dmg::ControlUnit::JR_Z(int8_t s8)
{
    if (m_cpu.F.read_z_flag() == 1)
    {
        m_cpu.PC.writeWord(m_cpu.PC.readWord() + s8);
    }
}

void dmg::ControlUnit::JR_C(int8_t s8)
{
    if (m_cpu.F.read_c_flag() == 1)
    {
        m_cpu.PC.writeWord(m_cpu.PC.readWord() + s8);
    }
}

void dmg::ControlUnit::JP()
{
    m_cpu.PC.writeWord(readA16());
}

void dmg::ControlUnit::JP_NZ()
{
    uint16_t a16 = readA16();
    if (m_cpu.F.read_z_flag() == 0)
    {
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::JP_NC()
{
    uint16_t a16 = readA16();
    if (m_cpu.F.read_c_flag() == 0)
    {
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::RST(uint8_t value)
{
    m_cpu.write_memory(--m_cpu.SP, m_cpu.PC.readHigh());
    m_cpu.write_memory(--m_cpu.SP, m_cpu.PC.readLow());
    m_cpu.PC.writeLow(value);
    m_cpu.PC.writeHigh(0x00);
}

void dmg::ControlUnit::LD(AByteRegister& dst, AByteRegister& src)
{
    dst.write(src.read());
}

void dmg::ControlUnit::LD(Address address, AByteRegister& src)
{
    m_cpu.write_memory(address.m_address, src.read());
}

void dmg::ControlUnit::LD(AByteRegister& reg, uint8_t d8)
{
    reg.write(d8);
}

void dmg::ControlUnit::LD(Address address, uint8_t d8)
{
    m_cpu.write_memory(address.m_address, d8);
}

void dmg::ControlUnit::LD(AByteRegister& reg, Address address)
{
    reg.write(m_cpu.read_memory(address.m_address));
}

void dmg::ControlUnit::LD(ATwoByteRegister& dst, uint16_t src)
{
    dst.writeWord(src);
}

void dmg::ControlUnit::LD_SP()
{
    uint16_t a16 = readA16();
    m_cpu.write_memory(a16, m_cpu.SP.readLow());
    m_cpu.write_memory(a16 + 1, m_cpu.SP.readHigh());
}

void dmg::ControlUnit::LD_internal()
{
    m_cpu.write_memory(make_uint16_t(0xFF, readA8()), m_cpu.A.read());
}

void dmg::ControlUnit::LD_internal_a()
{
    m_cpu.A.write(m_cpu.read_memory(make_uint16_t(0xFF, readA8())));
}

void dmg::ControlUnit::LD(ATwoByteRegister& reg)
{
    reg.writeWord(readD16());
}

void dmg::ControlUnit::ADD(AByteRegister& dst, AByteRegister& src)
{
    uint16_t result = dst.read() + src.read();

    m_cpu.F.set_z_flag((result & 0xFF) == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag((dst.read() & 0xF) + (src.read() & 0xF) > 0xF);

    dst.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::ADD(AByteRegister& dst, Address address)
{
    uint16_t result = dst.read() + m_cpu.read_memory(address.m_address);

    m_cpu.F.set_z_flag((result & 0xFF) == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag((dst.read() & 0xF) + (m_cpu.read_memory(address.m_address) & 0xF) > 0xF);

    dst.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::ADD(AByteRegister& dst, uint8_t d8)
{
    uint16_t result = dst.read() + d8;

    m_cpu.F.set_z_flag((result & 0xFF) == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag((dst.read() & 0xF) + (d8 & 0xF) > 0xF);

    dst.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::ADD(ATwoByteRegister& dst, int8_t s8)
{
    uint32_t result = dst.readWord() + s8;

    m_cpu.F.set_z_flag(false);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag((dst.readWord() & 0xF) + (s8 & 0xF) > 0xFF);

    dst.writeWord(static_cast<uint16_t>(result));
}

void dmg::ControlUnit::ADC(AByteRegister& dst, AByteRegister& src)
{
    uint8_t carry_flag = m_cpu.F.read_c_flag();
    uint16_t result = dst.read() + src.read() + carry_flag;

    m_cpu.F.set_z_flag((result & 0xFF) == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag(((dst.read() & 0xF) + (src.read() & 0xF) + carry_flag) > 0xF);

    dst.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::SUB(AByteRegister& a)
{
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - a.read());

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) - (a.read() & 0xF) < 0);

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::SUB(Address address)
{
    uint8_t memory_value = m_cpu.read_memory(address.m_address);
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - memory_value);

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) - (memory_value & 0xF) < 0);

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::SUB(uint8_t d8)
{
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - d8);

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) - (d8 & 0xF) < 0);

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::SBC(AByteRegister& reg)
{
    uint8_t carry_flag = m_cpu.F.read_c_flag();
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - reg.read() - carry_flag);

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) - (reg.read() & 0xF) - carry_flag  < 0);

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::AND(AByteRegister& a)
{
    uint8_t result = m_cpu.A.read() & a.read();

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(false);
    m_cpu.F.set_h_flag(true);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::AND(Address address)
{
    uint8_t address_value = m_cpu.read_memory(address.m_address);
    uint8_t result = m_cpu.A.read() & address_value;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(false);
    m_cpu.F.set_h_flag(true);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::AND(uint8_t d8)
{
    uint8_t result = m_cpu.A.read() & d8;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(false);
    m_cpu.F.set_h_flag(true);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::OR(AByteRegister& a)
{
    uint8_t result = m_cpu.A.read() | a.read();

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(false);
    m_cpu.F.set_h_flag(false);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::OR(Address address)
{
    uint8_t address_value = m_cpu.read_memory(address.m_address);
    uint8_t result = m_cpu.A.read() | address_value;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(false);
    m_cpu.F.set_h_flag(false);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::OR(uint8_t d8)
{
    uint8_t result = m_cpu.A.read() | d8;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(false);
    m_cpu.F.set_h_flag(false);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::XOR(AByteRegister& reg)
{
    uint8_t result = m_cpu.A.read() ^ reg.read();

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(false);
    m_cpu.F.set_h_flag(false);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::CP(AByteRegister& reg)
{
    uint8_t a_value = m_cpu.A.read();
    uint8_t reg_value = reg.read();

    m_cpu.F.set_z_flag(a_value == reg_value);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(a_value < reg_value);
    m_cpu.F.set_h_flag((a_value & 0xF) < (reg_value & 0xF));
}

void dmg::ControlUnit::INC(ATwoByteRegister& reg)
{
    ++reg;
}

void dmg::ControlUnit::INC(AByteRegister& reg)
{
    uint16_t result = reg.read() + 1;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_h_flag((reg.read() & 0xF + 1) > 0xF);
    //m_cpu.F.set_c_flag(result == 0);

    reg.write(result);
}

void dmg::ControlUnit::INC(Address address)
{
    uint8_t value = m_cpu.read_memory(address.m_address);
    uint16_t result = value + 1;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_h_flag((value & 0xF + 1) > 0xF);
    //m_cpu.F.set_c_flag(result == 0);

    m_cpu.write_memory(address.m_address, result);
}

void dmg::ControlUnit::DEC(AByteRegister& reg)
{
    uint8_t value = reg.read();
    uint8_t result = value - 1;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_h_flag(((value & 0xF) == 0));
    //m_cpu.F.set_c_flag(result == 0);

    reg.write(result);
}

void dmg::ControlUnit::DEC(Address address)
{
    uint8_t value = address.m_address;
    uint8_t result = value - 1;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_h_flag(((value & 0xF) == 0));
    //m_cpu.F.set_c_flag(result == 0);

    m_cpu.write_memory(address.m_address, result);
}

void dmg::ControlUnit::CALL_NZ()
{
    uint16_t a16 = readA16();
    if (m_cpu.F.read_z_flag() == 0)
    {
        --m_cpu.SP;
        m_cpu.write_memory(m_cpu.SP.readWord(), m_cpu.PC.readHigh());
        --m_cpu.SP;
        m_cpu.write_memory(m_cpu.SP.readWord(), m_cpu.PC.readLow());
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::CALL_NC()
{
    uint16_t a16 = readA16();
    if (m_cpu.F.read_c_flag() == 0)
    {
        --m_cpu.SP;
        m_cpu.write_memory(m_cpu.SP.readWord(), m_cpu.PC.readHigh());
        --m_cpu.SP;
        m_cpu.write_memory(m_cpu.SP.readWord(), m_cpu.PC.readLow());
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::RET_NZ()
{
    if (m_cpu.F.read_z_flag() == 0)
    {
        const uint8_t low = m_cpu.read_memory(m_cpu.SP++);
        const uint8_t high = m_cpu.read_memory(m_cpu.SP++);

        m_cpu.PC.writeWord((high << 8) | low);
    }
}

void dmg::ControlUnit::RET_NC()
{
    if (m_cpu.F.read_c_flag() == 0)
    {
        const uint8_t low = m_cpu.read_memory(m_cpu.SP++);
        const uint8_t high = m_cpu.read_memory(m_cpu.SP++);

        m_cpu.PC.writeWord((high << 8) | low);
    }
}

void dmg::ControlUnit::RET_Z()
{
    if (m_cpu.F.read_z_flag() == 1)
    {
        const uint8_t low = m_cpu.read_memory(m_cpu.SP++);
        const uint8_t high = m_cpu.read_memory(m_cpu.SP++);

        m_cpu.PC.writeWord((high << 8) | low);
    }
}

void dmg::ControlUnit::RET_C()
{
    if (m_cpu.F.read_c_flag() == 1)
    {
        const uint8_t low = m_cpu.read_memory(m_cpu.SP++);
        const uint8_t high = m_cpu.read_memory(m_cpu.SP++);

        m_cpu.PC.writeWord((high << 8) | low);
    }
}

void dmg::ControlUnit::POP(ATwoByteRegister& reg)
{
    reg.writeLow(m_cpu.read_memory(m_cpu.SP++));
    reg.writeHigh(m_cpu.read_memory(m_cpu.SP++));
}

void dmg::ControlUnit::PUSH(ATwoByteRegister& reg)
{
    m_cpu.write_memory(--m_cpu.SP, reg.readHigh());
    m_cpu.write_memory(--m_cpu.SP, reg.readLow());
}

uint8_t dmg::ControlUnit::readA8()
{
    return m_cpu.fetch();
}

uint16_t dmg::ControlUnit::readD16()
{
    uint8_t lsb = m_cpu.fetch();
    uint8_t msb = m_cpu.fetch();
    return (msb << 8) | lsb;
}

int8_t dmg::ControlUnit::readS8()
{
    return static_cast<int8_t>(m_cpu.fetch());
}

uint16_t dmg::ControlUnit::readA16()
{
    uint8_t lsb = m_cpu.fetch();
    uint8_t msb = m_cpu.fetch();
    return static_cast<uint16_t>((msb << 8) | lsb);
}

uint8_t dmg::ControlUnit::readD8()
{
    return m_cpu.fetch();
}
