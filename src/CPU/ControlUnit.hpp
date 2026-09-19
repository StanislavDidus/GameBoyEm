#pragma once

#include "Registers/AByteRegister.hpp"
#include "Registers/Address.hpp"
#include "Registers/ATwoByteRegister.hpp"

namespace dmg
{
    class CPU;

    class ControlUnit
    {
    public:
        explicit ControlUnit(CPU& cpu);

        void decode();
    private:
        void NOP();
        void STOP();
        void DI();
        void HALT();
        void RLCA();
        void RLA();
        void DAA();
        void SCF();

        void JR_NZ();
        void JR_NC();
        void JR();
        void JR_Z(int8_t s8);
        void JR_C(int8_t s8);
        void JP();
        void JP_NZ();
        void JP_NC();
        void RST(uint8_t value);

        void LD(AByteRegister& dst, AByteRegister& src);
        void LD(Address address, AByteRegister& src);
        void LD(AByteRegister& reg, uint8_t d8);
        void LD(Address address, uint8_t d8);
        void LD(AByteRegister& reg, Address address);
        void LD(ATwoByteRegister& dst, uint16_t src);
        void LD_SP();
        void LD_internal();
        void LD_internal_a();
        void LD(ATwoByteRegister& reg);

        void ADD(AByteRegister& dst, AByteRegister& src);
        void ADD(AByteRegister& dst, Address address);
        void ADD(AByteRegister& dst, uint8_t d8);
        void ADD(ATwoByteRegister& dst, int8_t s8);
        void ADC(AByteRegister& dst, AByteRegister& src);

        void SUB(AByteRegister& a);
        void SUB(Address address);
        void SUB(uint8_t d8);
        void SBC(AByteRegister& reg);

        void AND(AByteRegister& a);
        void AND(Address address);
        void AND(uint8_t d8);
        void OR(AByteRegister& a);
        void OR(Address address);
        void OR(uint8_t d8);
        void XOR(AByteRegister& reg);
        void CP(AByteRegister& reg);

        void INC(ATwoByteRegister& reg);
        void INC(AByteRegister& reg);
        void INC(Address address);
        void DEC(AByteRegister& reg);
        void DEC(Address address);

        void CALL_NZ();
        void CALL_NC();

        void RET_NZ();
        void RET_NC();
        void RET_Z();
        void RET_C();

        void POP(ATwoByteRegister& reg);
        void PUSH(ATwoByteRegister& reg);

        uint8_t readA8();
        uint16_t readD16();
        int8_t readS8();
        uint16_t readA16();
        uint8_t readD8();

        CPU& m_cpu;
    };
}
