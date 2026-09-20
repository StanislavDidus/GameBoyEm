#include "ControlUnit.hpp"

#include <filesystem>

#include "Utils/Log.hpp"
#include "CPU.hpp"
#include "Utils/Math.hpp"

dmg::ControlUnit::ControlUnit(CPU& cpu)
    : m_cpu(cpu)
{
}

void dmg::ControlUnit::Decode()
{
    const uint8_t opcode = m_cpu.ReadMemory(m_cpu.PC++);

    //if (opcode != 0)
        //printf("cool\n");
    DMG_INFO("Decoding instruction: {:08x} at {:08x}\n", opcode, m_cpu.PC.readWord() - 1);

    switch (opcode)
    {
        // 0x0 Column
        case 0x00: NOP(); break;                                      // NOP
        case 0x10: STOP(); break;                                     // STOP
        case 0x20: JR_NZ(); break;                                    // JR NZ, s8
        case 0x30: JR_NC(); break;                                    // JR NC, s8
        case 0x40: LD(m_cpu.B, m_cpu.B); break;                       // LD B, B
        case 0x50: LD(m_cpu.D, m_cpu.B); break;                       // LD D, B
        case 0x60: LD(m_cpu.H, m_cpu.B); break;                       // LD H, B
        case 0x70: LD(Address(m_cpu.HL.readWord()), m_cpu.B); break;  // LD (HL), B
        case 0x80: ADD(m_cpu.A, m_cpu.B); break;                      // ADD A, B
        case 0x90: SUB(m_cpu.B); break;                               // SUB B
        case 0xA0: AND(m_cpu.B); break;                               // AND B
        case 0xB0: OR(m_cpu.B); break;                                // OR B
        case 0xC0: RET_NZ(); break;                                   // RET NZ
        case 0xD0: RET_NC(); break;                                   // RET NC
        case 0xE0: LD_internal(); break;                              // LD (a8), A
        case 0xF0: LD_internal_a(); break;                            // LD A, (a8)

        // 0x1 Column
        case 0x01: LD(m_cpu.BC); break;                               // LD BC, d16
        case 0x11: LD(m_cpu.DE); break;                               // LD DE, d16
        case 0x21: LD(m_cpu.HL); break;                               // LD HL, d16
        case 0x31: LD(m_cpu.SP); break;                               // LD SP, d16
        case 0x41: LD(m_cpu.B, m_cpu.C); break;                       // LD B, C
        case 0x51: LD(m_cpu.D, m_cpu.C); break;                       // LD D, C
        case 0x61: LD(m_cpu.H, m_cpu.C); break;                       // LD H, C
        case 0x71: LD(Address(m_cpu.HL), m_cpu.C); break;             // LD (HL), C
        case 0x81: ADD(m_cpu.A, m_cpu.C); break;                      // ADD A, C
        case 0x91: SUB(m_cpu.C); break;                               // SUB C
        case 0xA1: AND(m_cpu.C); break;                               // AND C
        case 0xB1: OR(m_cpu.C); break;                                // OR C
        case 0xC1: POP(m_cpu.BC); break;                              // POP BC
        case 0xD1: POP(m_cpu.DE); break;                              // POP DE
        case 0xE1: POP(m_cpu.HL); break;                              // POP HL
        case 0xF1: POP(m_cpu.AF); break;                              // POP AF

        // 0x2 Column
        case 0x02: LD(Address(m_cpu.BC), m_cpu.A); break;             // LD (BC), A
        case 0x12: LD(Address(m_cpu.DE), m_cpu.A); break;             // LD (DE), A
        case 0x22: LD(Address(m_cpu.HL++), m_cpu.A); break;           // LD (HL+), A
        case 0x32: LD(Address(m_cpu.HL--), m_cpu.A); break;           // LD (HL-), A
        case 0x42: LD(m_cpu.B, m_cpu.D); break;                       // LD B, D
        case 0x52: LD(m_cpu.D, m_cpu.D); break;                       // LD D, D
        case 0x62: LD(m_cpu.H, m_cpu.D); break;                       // LD H, D
        case 0x72: LD(Address(m_cpu.HL), m_cpu.D); break;             // LD (HL), D
        case 0x82: ADD(m_cpu.A, m_cpu.D); break;                      // ADD A, D
        case 0x92: SUB(m_cpu.D); break;                               // SUB D
        case 0xA2: AND(m_cpu.D); break;                               // AND D
        case 0xB2: OR(m_cpu.D); break;                                // OR D
        case 0xC2: JP_NZ(); break;                                    // JP NZ, a16
        case 0xD2: JP_NC(); break;                                    // JP NC, a16
        case 0xE2: LD_internal(); break;                              // LD (C), A
        case 0xF2: LD_internal_a(); break;                            // LD A, (C)

        // 0x3 Column
        case 0x03: INC(m_cpu.BC); break;                              // INC BC
        case 0x13: INC(m_cpu.DE); break;                              // INC DE
        case 0x23: INC(m_cpu.HL); break;                              // INC HL
        case 0x33: INC(m_cpu.SP); break;                              // INC SP
        case 0x43: LD(m_cpu.B, m_cpu.E); break;                       // LD B, E
        case 0x53: LD(m_cpu.D, m_cpu.E); break;                       // LD D, E
        case 0x63: LD(m_cpu.H, m_cpu.E); break;                       // LD H, E
        case 0x73: LD(Address(m_cpu.HL), m_cpu.E); break;             // LD (HL), E
        case 0x83: ADD(m_cpu.A, m_cpu.E); break;                      // ADD A, E
        case 0x93: SUB(m_cpu.E); break;                               // SUB E
        case 0xA3: AND(m_cpu.E); break;                               // AND E
        case 0xB3: OR(m_cpu.E); break;                                // OR E
        case 0xC3: JP(); break;                                       // JP a16
        case 0xD3: DMG_WARN("Instruction 0xD3 is non-existent.\n"); break;
        case 0xE3: DMG_WARN("Instruction 0xE3 is non-existent.\n"); break;
        case 0xF3: DI(); break;                                       // DI

        // 0x4 Column
        case 0x04: INC(m_cpu.B); break;                               // INC B
        case 0x14: INC(m_cpu.D); break;                               // INC D
        case 0x24: INC(m_cpu.H); break;                               // INC H
        case 0x34: INC(Address(m_cpu.HL)); break;                     // INC (HL)
        case 0x44: LD(m_cpu.B, m_cpu.H); break;                       // LD B, H
        case 0x54: LD(m_cpu.D, m_cpu.H); break;                       // LD D, H
        case 0x64: LD(m_cpu.H, m_cpu.H); break;                       // LD H, H
        case 0x74: LD(Address(m_cpu.HL), m_cpu.H); break;             // LD (HL), H
        case 0x84: ADD(m_cpu.A, m_cpu.H); break;                      // ADD A, H
        case 0x94: SUB(m_cpu.H); break;                               // SUB H
        case 0xA4: AND(m_cpu.H); break;                               // AND H
        case 0xB4: OR(m_cpu.H); break;                                // OR H
        case 0xC4: CALL_NZ(); break;                                  // CALL NZ, a16
        case 0xD4: CALL_NC(); break;                                  // CALL NC, a16

        // 0x5 Column
        case 0x05: DEC(m_cpu.B); break;                               // DEC B
        case 0x15: DEC(m_cpu.D); break;                               // DEC D
        case 0x25: DEC(m_cpu.H); break;                               // DEC H
        case 0x35: DEC(Address(m_cpu.HL)); break;                     // DEC (HL)
        case 0x45: LD(m_cpu.B, m_cpu.L); break;                       // LD B, L
        case 0x55: LD(m_cpu.D, m_cpu.L); break;                       // LD D, L
        case 0x65: LD(m_cpu.H, m_cpu.L); break;                       // LD H, L
        case 0x75: LD(Address(m_cpu.HL), m_cpu.L); break;             // LD (HL), L
        case 0x85: ADD(m_cpu.A, m_cpu.L); break;                      // ADD A, L
        case 0x95: SUB(m_cpu.L); break;                               // SUB L
        case 0xA5: AND(m_cpu.L); break;                               // AND L
        case 0xB5: OR(m_cpu.L); break;                                // OR L
        case 0xC5: PUSH(m_cpu.BC); break;                             // PUSH BC
        case 0xD5: PUSH(m_cpu.DE); break;                             // PUSH DE
        case 0xE5: PUSH(m_cpu.HL); break;                             // PUSH HL
        case 0xF5: PUSH(m_cpu.AF); break;                             // PUSH AF

        // 0x6 Column
        case 0x06: LD(m_cpu.B, ReadD8()); break;                      // LD B, d8
        case 0x16: LD(m_cpu.D, ReadD8()); break;                      // LD D, d8
        case 0x26: LD(m_cpu.H, ReadD8()); break;                      // LD H, d8
        case 0x36: LD(Address(m_cpu.HL), ReadD8()); break;            // LD (HL), d8
        case 0x46: LD(m_cpu.B, Address(m_cpu.HL)); break;             // LD B, (HL)
        case 0x56: LD(m_cpu.D, Address(m_cpu.HL)); break;             // LD D, (HL)
        case 0x66: LD(m_cpu.H, Address(m_cpu.HL)); break;             // LD H, (HL)
        case 0x76: HALT(); break;                                     // HALT
        case 0x86: ADD(m_cpu.A, Address(m_cpu.HL)); break;            // ADD A, (HL)
        case 0x96: SUB(Address(m_cpu.HL)); break;                     // SUB (HL)
        case 0xA6: AND(Address(m_cpu.HL)); break;                     // AND (HL)
        case 0xB6: OR(Address(m_cpu.HL)); break;                      // OR (HL)
        case 0xC6: ADD(m_cpu.A, ReadD8()); break;                     // ADD A, d8
        case 0xD6: SUB(ReadD8()); break;                              // SUB d8
        case 0xE6: AND(ReadD8()); break;                              // AND d8
        case 0xF6: OR(ReadD8()); break;                               // OR d8

        // 0x7 Column
        case 0x07: RLCA(); break;                                     // RLCA
        case 0x17: RLA(); break;                                      // RLA
        case 0x27: DAA(); break;                                      // DAA
        case 0x37: SCF(); break;                                      // SCF
        case 0x47: LD(m_cpu.B, m_cpu.A); break;                       // LD B, A
        case 0x57: LD(m_cpu.D, m_cpu.A); break;                       // LD D, A
        case 0x67: LD(m_cpu.H, m_cpu.A); break;                       // LD H, A
        case 0x77: LD(Address(m_cpu.HL), m_cpu.A); break;             // LD (HL), A
        case 0x87: ADD(m_cpu.A, m_cpu.A); break;                      // ADD A, A
        case 0x97: SUB(m_cpu.A); break;                               // SUB A
        case 0xA7: AND(m_cpu.A); break;                               // AND A
        case 0xB7: OR(m_cpu.A); break;                                // OR A
        case 0xC7: RST(0x00); break;                                  // RST 0
        case 0xD7: RST(0x10); break;                                  // RST 2
        case 0xE7: RST(0x20); break;                                  // RST 4
        case 0xF7: RST(0x30); break;                                  // RST 6

        // 0x8 Column
        case 0x08: LD_SP(); break;                                    // LD (a16), SP
        case 0x18: JR(); break;                                       // JR s8
        case 0x28: JR_Z(ReadS8()); break;                             // JR Z, s8
        case 0x38: JR_C(ReadS8()); break;                             // JR C, s8
        case 0x48: LD(m_cpu.C, m_cpu.B); break;                       // LD C, B
        case 0x58: LD(m_cpu.E, m_cpu.B); break;                       // LD E, B
        case 0x68: LD(m_cpu.L, m_cpu.B); break;                       // LD L, B
        case 0x78: LD(m_cpu.A, m_cpu.B); break;                       // LD A, B
        case 0x88: ADC(m_cpu.B); break;                               // ADC A, B
        case 0x98: SBC(m_cpu.B); break;                               // SBC A, B
        case 0xA8: XOR(m_cpu.B); break;                               // XOR B
        case 0xB8: CP(m_cpu.B); break;                                // CP B
        case 0xC8: RET_Z(); break;                                    // RET Z
        case 0xD8: RET_C(); break;                                    // RET C
        case 0xE8: ADD(m_cpu.SP, ReadS8()); break;                    // ADD SP, s8
        case 0xF8: LD(m_cpu.HL, m_cpu.SP.readWord() + ReadS8()); break; // LD HL, SP+s8

        // 0x9 Column
        case 0x09: ADD(m_cpu.HL, m_cpu.BC); break;                    // ADD HL, BC
        case 0x19: ADD(m_cpu.HL, m_cpu.DE); break;                    // ADD HL, DE
        case 0x29: ADD(m_cpu.HL, m_cpu.HL); break;                    // ADD HL, HL
        case 0x39: ADD(m_cpu.HL, m_cpu.SP); break;                    // ADD HL, SP
        case 0x49: LD(m_cpu.C, m_cpu.C); break;                       // LD C, C
        case 0x59: LD(m_cpu.E, m_cpu.C); break;                       // LD E, C
        case 0x69: LD(m_cpu.L, m_cpu.C); break;                       // LD L, C
        case 0x79: LD(m_cpu.A, m_cpu.C); break;                       // LD A, C
        case 0x89: ADC(m_cpu.C); break;                               // ADC A, C
        case 0x99: SBC(m_cpu.C); break;                               // SBC A, C
        case 0xA9: XOR(m_cpu.C); break;                               // XOR C
        case 0xB9: CP(m_cpu.C); break;                                // CP C
        case 0xC9: RET(); break;                                      // RET
        case 0xD9: RETI(); break;                                     // RETI
        case 0xE9: JP(m_cpu.HL); break;                               // JP HL
        case 0xF9: LD(m_cpu.SP, m_cpu.HL.readWord()); break;          // LD SP, HL

        // 0xA Column
        case 0x0A: LD(m_cpu.A, Address(m_cpu.BC)); break;             // LD A, (BC)
        case 0x1A: LD(m_cpu.A, Address(m_cpu.DE)); break;             // LD A, (DE)
        case 0x2A: LD(m_cpu.A, Address(m_cpu.HL++)); break;           // LD A, (HL+)
        case 0x3A: LD(m_cpu.A, Address(m_cpu.HL--)); break;           // LD A, (HL-)
        case 0x4A: LD(m_cpu.C, m_cpu.D); break;                       // LD C, D
        case 0x5A: LD(m_cpu.E, m_cpu.D); break;                       // LD E, D
        case 0x6A: LD(m_cpu.L, m_cpu.D); break;                       // LD L, D
        case 0x7A: LD(m_cpu.A, m_cpu.D); break;                       // LD A, D
        case 0x8A: ADC(m_cpu.D); break;                               // ADC A, D
        case 0x9A: SBC(m_cpu.D); break;                               // SBC A, D
        case 0xAA: XOR(m_cpu.D); break;                               // XOR D
        case 0xBA: CP(m_cpu.D); break;                                // CP D
        case 0xCA: JP_Z(); break;                                     // JP Z, a16
        case 0xDA: JP_C(); break;                                     // JP C, a16
        case 0xEA: LD(Address(ReadA16()), m_cpu.A); break;            // LD (a16), A
        case 0xFA: LD(m_cpu.A, Address(ReadA16())); break;            // LD A, (a16)

        // 0xB Column
        case 0x0B: DEC(m_cpu.BC); break;                              // DEC BC
        case 0x1B: DEC(m_cpu.DE); break;                              // DEC DE
        case 0x2B: DEC(m_cpu.HL); break;                              // DEC HL
        case 0x3B: DEC(m_cpu.SP); break;                              // DEC SP
        case 0x4B: LD(m_cpu.C, m_cpu.E); break;                       // LD C, E
        case 0x5B: LD(m_cpu.E, m_cpu.E); break;                       // LD E, E
        case 0x6B: LD(m_cpu.L, m_cpu.E); break;                       // LD L, E
        case 0x7B: LD(m_cpu.A, m_cpu.E); break;                       // LD A, E
        case 0x8B: ADC(m_cpu.E); break;                               // ADC A, E
        case 0x9B: SBC(m_cpu.E); break;                               // SBC A, E
        case 0xAB: XOR(m_cpu.E); break;                               // XOR E
        case 0xBB: CP(m_cpu.E); break;                                // CP E
        case 0xCB: DMG_WARN("CB table is not implemented yet.\n"); break;
        case 0xDB: DMG_WARN("Instruction 0xDB is non-existent.\n"); break;
        case 0xEB: DMG_WARN("Instruction 0xEB is non-existent.\n"); break;
        case 0xFB: EI(); break;                                       // EI

        // 0xC Column
        case 0x0C: INC(m_cpu.C); break;                               // INC C
        case 0x1C: INC(m_cpu.E); break;                               // INC E
        case 0x2C: INC(m_cpu.L); break;                               // INC L
        case 0x3C: INC(m_cpu.A); break;                               // INC A
        case 0x4C: LD(m_cpu.C, m_cpu.H); break;                       // LD C, H
        case 0x5C: LD(m_cpu.E, m_cpu.H); break;                       // LD E, H
        case 0x6C: LD(m_cpu.L, m_cpu.H); break;                       // LD L, H
        case 0x7C: LD(m_cpu.A, m_cpu.H); break;                       // LD A, H
        case 0x8C: ADC(m_cpu.H); break;                               // ADC A, H
        case 0x9C: SBC(m_cpu.H); break;                               // SBC A, H
        case 0xAC: XOR(m_cpu.H); break;                               // XOR H
        case 0xBC: CP(m_cpu.H); break;                                // CP H
        case 0xCC: CALL_Z(); break;                                   // CALL Z, a16
        case 0xDC: CALL_C(); break;                                   // CALL C, a16
        case 0xEC: DMG_WARN("Instruction 0xEC is non-existent.\n"); break;
        case 0xFC: DMG_WARN("Instruction 0xFC is non-existent.\n"); break;

        // 0xD Column
        case 0x0D: DEC(m_cpu.C); break;                               // DEC C
        case 0x1D: DEC(m_cpu.E); break;                               // DEC E
        case 0x2D: DEC(m_cpu.L); break;                               // DEC L
        case 0x3D: DEC(m_cpu.A); break;                               // DEC A
        case 0x4D: LD(m_cpu.C, m_cpu.L); break;                       // LD C, L
        case 0x5D: LD(m_cpu.E, m_cpu.L); break;                       // LD E, L
        case 0x6D: LD(m_cpu.L, m_cpu.L); break;                       // LD L, L
        case 0x7D: LD(m_cpu.A, m_cpu.L); break;                       // LD A, L
        case 0x8D: ADC(m_cpu.L); break;                               // ADC A, L
        case 0x9D: SBC(m_cpu.L); break;                               // SBC A, L
        case 0xAD: XOR(m_cpu.L); break;                               // XOR L
        case 0xBD: CP(m_cpu.L); break;                                // CP L
        case 0xCD: CALL(); break;                                     // CALL a16
        case 0xDD: DMG_WARN("Instruction 0xDD is non-existent.\n"); break;
        case 0xED: DMG_WARN("Instruction 0xED is non-existent.\n"); break;
        case 0xFD: DMG_WARN("Instruction 0xFD is non-existent.\n"); break;

        // 0xE Column
        case 0x0E: LD(m_cpu.C, ReadD8()); break;                      // LD C, d8
        case 0x1E: LD(m_cpu.E, ReadD8()); break;                      // LD E, d8
        case 0x2E: LD(m_cpu.L, ReadD8()); break;                      // LD L, d8
        case 0x3E: LD(m_cpu.A, ReadD8()); break;                      // LD A, d8
        case 0x4E: LD(m_cpu.C, Address(m_cpu.HL)); break;             // LD C, (HL)
        case 0x5E: LD(m_cpu.E, Address(m_cpu.HL)); break;             // LD E, (HL)
        case 0x6E: LD(m_cpu.L, Address(m_cpu.HL)); break;             // LD L, (HL)
        case 0x7E: LD(m_cpu.A, Address(m_cpu.HL)); break;             // LD A, (HL)
        case 0x8E: ADC(Address(m_cpu.HL)); break;                     // ADC A, (HL)
        case 0x9E: SBC(Address(m_cpu.HL)); break;                     // SBC A, (HL)
        case 0xAE: XOR(Address(m_cpu.HL)); break;                     // XOR (HL)
        case 0xBE: CP(Address(m_cpu.HL)); break;                      // CP (HL)
        case 0xCE: ADC(ReadD8()); break;                              // ADC A, d8
        case 0xDE: SBC(ReadD8()); break;                              // SBC A, d8
        case 0xEE: XOR(ReadD8()); break;                              // XOR d8
        case 0xFE: CP(ReadD8()); break;                               // CP d8

        // 0xF Column
        case 0x0F: RRCA(); break;                                     // RRCA
        case 0x1F: RRA(); break;                                      // RRA
        case 0x2F: CPL(); break;                                      // CPL
        case 0x3F: CCF(); break;                                      // CCF
        case 0x4F: LD(m_cpu.C, m_cpu.A); break;                       // LD C, A
        case 0x5F: LD(m_cpu.E, m_cpu.A); break;                       // LD E, A
        case 0x6F: LD(m_cpu.L, m_cpu.A); break;                       // LD L, A
        case 0x7F: LD(m_cpu.A, m_cpu.A); break;                       // LD A, A
        case 0x8F: ADC(m_cpu.A); break;                               // ADC A, A
        case 0x9F: SBC(m_cpu.A); break;                               // SBC A, A
        case 0xAF: XOR(m_cpu.A); break;                               // XOR A
        case 0xBF: CP(m_cpu.A); break;                                // CP A
        case 0xCF: RST(0x08); break;                                  // RST 1
        case 0xDF: RST(0x18); break;                                  // RST 3
        case 0xEF: RST(0x28); break;                                  // RST 5
        case 0xFF: RST(0x38); break;                                  // RST 7

        default:
            DMG_WARN("There is no matching instruction for this opcode: %d\n", opcode);
            break;
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

void dmg::ControlUnit::EI()
{
    DMG_WARN("EI function is not yet implemented.\n");
}

void dmg::ControlUnit::RRCA()
{
    uint8_t value = m_cpu.A.read();

    uint8_t last_bit = getBit(value, 0);

    m_cpu.F.set_z_flag(false);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_h_flag(false);
    m_cpu.F.set_c_flag((last_bit == 1));

    value = value >> 1;

    setBit(value, 7, last_bit);
    m_cpu.A.write(value);
}

void dmg::ControlUnit::RRA()
{
    uint8_t value = m_cpu.A.read();

    uint8_t last_bit = getBit(value, 0);
    uint8_t old_carry_value = m_cpu.F.read_c_flag();

    m_cpu.F.set_z_flag(false);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_h_flag(false);
    m_cpu.F.set_c_flag((last_bit == 1));

    value = value >> 1;

    setBit(value, 7, old_carry_value);
    m_cpu.A.write(value);
}

void dmg::ControlUnit::CPL()
{
    m_cpu.A.write(~m_cpu.A.read());
}

void dmg::ControlUnit::CCF()
{
    m_cpu.F.set_c_flag(~m_cpu.F.read_c_flag());
}

void dmg::ControlUnit::JR_NZ()
{
    int8_t s8 = ReadS8();
    if (m_cpu.F.read_z_flag() == 0)
    {
        m_cpu.PC.increase(s8);
    }
}

void dmg::ControlUnit::JR_NC()
{
    int8_t s8 = ReadS8();
    if (m_cpu.F.read_c_flag() == 0)
    {
        m_cpu.PC.increase(s8);
    }
}

void dmg::ControlUnit::JR()
{
    m_cpu.PC.writeWord(m_cpu.PC.readWord() + ReadS8());
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
    m_cpu.PC.writeWord(ReadA16());
}

void dmg::ControlUnit::JP(ATwoByteRegister& reg)
{
    m_cpu.PC.writeWord(reg.readWord());
}

void dmg::ControlUnit::JP_NZ()
{
    uint16_t a16 = ReadA16();
    if (m_cpu.F.read_z_flag() == 0)
    {
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::JP_NC()
{
    uint16_t a16 = ReadA16();
    if (m_cpu.F.read_c_flag() == 0)
    {
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::JP_Z()
{
    uint16_t a16 = ReadA16();
    if (m_cpu.F.read_z_flag() == 1)
    {
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::JP_C()
{
    uint16_t a16 = ReadA16();
    if (m_cpu.F.read_c_flag() == 1)
    {
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::RST(uint8_t value)
{
    m_cpu.WriteMemory(--m_cpu.SP, m_cpu.PC.readHigh());
    m_cpu.WriteMemory(--m_cpu.SP, m_cpu.PC.readLow());
    m_cpu.PC.writeLow(value);
    m_cpu.PC.writeHigh(0x00);
}

void dmg::ControlUnit::LD(AByteRegister& dst, AByteRegister& src)
{
    dst.write(src.read());
}

void dmg::ControlUnit::LD(Address address, AByteRegister& src)
{
    m_cpu.WriteMemory(address.m_address, src.read());
}

void dmg::ControlUnit::LD(AByteRegister& reg, uint8_t d8)
{
    reg.write(d8);
}

void dmg::ControlUnit::LD(Address address, uint8_t d8)
{
    m_cpu.WriteMemory(address.m_address, d8);
}

void dmg::ControlUnit::LD(AByteRegister& reg, Address address)
{
    reg.write(m_cpu.ReadMemory(address.m_address));
}

void dmg::ControlUnit::LD(ATwoByteRegister& dst, uint16_t src)
{
    dst.writeWord(src);
}

void dmg::ControlUnit::LD_SP()
{
    uint16_t a16 = ReadA16();
    m_cpu.WriteMemory(a16, m_cpu.SP.readLow());
    m_cpu.WriteMemory(a16 + 1, m_cpu.SP.readHigh());
}

void dmg::ControlUnit::LD_internal()
{
    m_cpu.WriteMemory(make_uint16_t(0xFF, ReadA8()), m_cpu.A.read());
}

void dmg::ControlUnit::LD_internal_a()
{
    m_cpu.A.write(m_cpu.ReadMemory(make_uint16_t(0xFF, ReadA8())));
}

void dmg::ControlUnit::LD(ATwoByteRegister& reg)
{
    reg.writeWord(ReadD16());
}

void dmg::ControlUnit::ADD(AByteRegister& dst, const AByteRegister& src)
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
    uint16_t result = dst.read() + m_cpu.ReadMemory(address.m_address);

    m_cpu.F.set_z_flag((result & 0xFF) == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag((dst.read() & 0xF) + (m_cpu.ReadMemory(address.m_address) & 0xF) > 0xF);

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
    m_cpu.F.set_h_flag((dst.readWord() & 0xF) + (s8 & 0xF) > 0xF);

    dst.writeWord(static_cast<uint16_t>(result));
}

void dmg::ControlUnit::ADD(ATwoByteRegister& dst, ATwoByteRegister& src)
{
    uint32_t result = dst.readWord() + src.readWord();

    m_cpu.F.set_z_flag(false);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag((dst.readWord() & 0xF) + (src.readWord() & 0xF) > 0xF);

    dst.writeWord(static_cast<uint16_t>(result));
}

void dmg::ControlUnit::ADC(AByteRegister& src)
{
    uint8_t carry_flag = m_cpu.F.read_c_flag();
    uint16_t result = m_cpu.A.read() + src.read() + carry_flag;

    m_cpu.F.set_z_flag((result & 0xFF) == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag(((m_cpu.A.read() & 0xF) + (src.read() & 0xF) + carry_flag) > 0xF);

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::ADC(Address address)
{
    uint8_t carry_flag = m_cpu.F.read_c_flag();
    uint8_t address_value = m_cpu.ReadMemory(address.m_address);
    uint16_t result = m_cpu.A.read() + address_value + carry_flag;

    m_cpu.F.set_z_flag((result & 0xFF) == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag(((m_cpu.A.read() & 0xF) + (address_value & 0xF) + carry_flag) > 0xF);

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::ADC(uint8_t d8)
{
    uint8_t carry_flag = m_cpu.F.read_c_flag();
    uint16_t result = m_cpu.A.read() + d8 + carry_flag;

    m_cpu.F.set_z_flag((result & 0xFF) == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(result > 0xFF);
    m_cpu.F.set_h_flag(((m_cpu.A.read() & 0xF) + (d8 & 0xF) + carry_flag) > 0xF);

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::SUB(AByteRegister& a)
{
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - a.read());

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) < (a.read() & 0xF));

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::SUB(Address address)
{
    uint8_t memory_value = m_cpu.ReadMemory(address.m_address);
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - memory_value);

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) < (memory_value & 0xF));

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::SUB(uint8_t d8)
{
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - d8);

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) < (d8 & 0xF));

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

void dmg::ControlUnit::SBC(Address address)
{
    uint8_t carry_flag = m_cpu.F.read_c_flag();
    uint8_t address_value = m_cpu.ReadMemory(address.m_address);
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - address_value - carry_flag);

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) - (address_value & 0xF) - carry_flag  < 0);

    m_cpu.A.write(static_cast<uint8_t>(result));
}

void dmg::ControlUnit::SBC(uint8_t d8)
{
    uint8_t carry_flag = m_cpu.F.read_c_flag();
    int16_t result = static_cast<int16_t>(m_cpu.A.read() - d8 - carry_flag);

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(result < 0);
    m_cpu.F.set_h_flag((m_cpu.A.read() & 0xF) - (d8 & 0xF) - carry_flag  < 0);

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
    uint8_t address_value = m_cpu.ReadMemory(address.m_address);
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
    uint8_t address_value = m_cpu.ReadMemory(address.m_address);
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

void dmg::ControlUnit::XOR(Address address)
{
    uint8_t result = m_cpu.A.read() ^ m_cpu.ReadMemory(address.m_address);

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_c_flag(false);
    m_cpu.F.set_h_flag(false);

    m_cpu.A.write(result);
}

void dmg::ControlUnit::XOR(uint8_t d8)
{
    uint8_t result = m_cpu.A.read() ^ d8;

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

void dmg::ControlUnit::CP(Address address)
{
    const uint8_t a_value = m_cpu.A.read();
    const uint8_t reg_value = m_cpu.ReadMemory(address.m_address);

    m_cpu.F.set_z_flag(a_value == reg_value);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(a_value < reg_value);
    m_cpu.F.set_h_flag((a_value & 0xF) < (reg_value & 0xF));
}

void dmg::ControlUnit::CP(uint8_t d8)
{
    uint8_t a_value = m_cpu.A.read();

    m_cpu.F.set_z_flag(a_value == d8);
    m_cpu.F.set_n_flag(true);
    m_cpu.F.set_c_flag(a_value < d8);
    m_cpu.F.set_h_flag((a_value & 0xF) < (d8 & 0xF));
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
    uint8_t value = m_cpu.ReadMemory(address.m_address);
    uint16_t result = value + 1;

    m_cpu.F.set_z_flag(result == 0);
    m_cpu.F.set_n_flag(false);
    m_cpu.F.set_h_flag((value & 0xF + 1) > 0xF);
    //m_cpu.F.set_c_flag(result == 0);

    m_cpu.WriteMemory(address.m_address, result);
}

void dmg::ControlUnit::DEC(ATwoByteRegister& reg)
{
    uint16_t result = reg.readWord() - 1;
    reg.writeWord(result);
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

    m_cpu.WriteMemory(address.m_address, result);
}

void dmg::ControlUnit::CALL_NZ()
{
    uint16_t a16 = ReadA16();
    if (m_cpu.F.read_z_flag() == 0)
    {
        --m_cpu.SP;
        m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readHigh());
        --m_cpu.SP;
        m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readLow());
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::CALL_NC()
{
    uint16_t a16 = ReadA16();
    if (m_cpu.F.read_c_flag() == 0)
    {
        --m_cpu.SP;
        m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readHigh());
        --m_cpu.SP;
        m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readLow());
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::CALL_Z()
{
    uint16_t a16 = ReadA16();
    if (m_cpu.F.read_z_flag() == 1)
    {
        --m_cpu.SP;
        m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readHigh());
        --m_cpu.SP;
        m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readLow());
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::CALL_C()
{
    uint16_t a16 = ReadA16();
    if (m_cpu.F.read_c_flag() == 1)
    {
        --m_cpu.SP;
        m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readHigh());
        --m_cpu.SP;
        m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readLow());
        m_cpu.PC.writeWord(a16);
    }
}

void dmg::ControlUnit::CALL()
{
    uint16_t a16 = ReadA16();

    --m_cpu.SP;
    m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readHigh());
    --m_cpu.SP;
    m_cpu.WriteMemory(m_cpu.SP.readWord(), m_cpu.PC.readLow());
    m_cpu.PC.writeWord(a16);
}

void dmg::ControlUnit::RET_NZ()
{
    if (m_cpu.F.read_z_flag() == 0)
    {
        const uint8_t low = m_cpu.ReadMemory(m_cpu.SP++);
        const uint8_t high = m_cpu.ReadMemory(m_cpu.SP++);

        m_cpu.PC.writeWord((high << 8) | low);
    }
}

void dmg::ControlUnit::RET_NC()
{
    if (m_cpu.F.read_c_flag() == 0)
    {
        const uint8_t low = m_cpu.ReadMemory(m_cpu.SP++);
        const uint8_t high = m_cpu.ReadMemory(m_cpu.SP++);

        m_cpu.PC.writeWord((high << 8) | low);
    }
}

void dmg::ControlUnit::RET_Z()
{
    if (m_cpu.F.read_z_flag() == 1)
    {
        const uint8_t low = m_cpu.ReadMemory(m_cpu.SP++);
        const uint8_t high = m_cpu.ReadMemory(m_cpu.SP++);

        m_cpu.PC.writeWord((high << 8) | low);
    }
}

void dmg::ControlUnit::RET_C()
{
    if (m_cpu.F.read_c_flag() == 1)
    {
        const uint8_t low = m_cpu.ReadMemory(m_cpu.SP++);
        const uint8_t high = m_cpu.ReadMemory(m_cpu.SP++);

        m_cpu.PC.writeWord((high << 8) | low);
    }
}

void dmg::ControlUnit::RET()
{
    const uint8_t low = m_cpu.ReadMemory(m_cpu.SP++);
    const uint8_t high = m_cpu.ReadMemory(m_cpu.SP++);

    m_cpu.PC.writeWord((high << 8) | low);
}

void dmg::ControlUnit::RETI()
{
    const uint8_t low = m_cpu.ReadMemory(m_cpu.SP++);
    const uint8_t high = m_cpu.ReadMemory(m_cpu.SP++);

    m_cpu.PC.writeWord((high << 8) | low);
}

void dmg::ControlUnit::POP(ATwoByteRegister& reg)
{
    reg.writeLow(m_cpu.ReadMemory(m_cpu.SP++));
    reg.writeHigh(m_cpu.ReadMemory(m_cpu.SP++));
}

void dmg::ControlUnit::PUSH(ATwoByteRegister& reg)
{
    m_cpu.WriteMemory(--m_cpu.SP, reg.readHigh());
    m_cpu.WriteMemory(--m_cpu.SP, reg.readLow());
}

uint8_t dmg::ControlUnit::ReadA8()
{
    return m_cpu.Fetch();
}

uint16_t dmg::ControlUnit::ReadD16()
{
    uint8_t lsb = m_cpu.Fetch();
    uint8_t msb = m_cpu.Fetch();
    return (msb << 8) | lsb;
}

int8_t dmg::ControlUnit::ReadS8()
{
    return static_cast<int8_t>(m_cpu.Fetch());
}

uint16_t dmg::ControlUnit::ReadA16()
{
    uint8_t lsb = m_cpu.Fetch();
    uint8_t msb = m_cpu.Fetch();
    return static_cast<uint16_t>((msb << 8) | lsb);
}

uint8_t dmg::ControlUnit::ReadD8()
{
    return m_cpu.Fetch();
}
