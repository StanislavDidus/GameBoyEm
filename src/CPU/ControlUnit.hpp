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

        void Decode();
    private:
        void NOP();
        void STOP();
        void DI();
        void HALT();
        void RLCA();
        void RLA();
        void DAA();
        void SCF();
        void EI();
        void RRCA();
        void RRA();
        void CPL();
        void CCF();

        void JR_NZ();
        void JR_NC();
        void JR();
        void JR_Z(int8_t s8);
        void JR_C(int8_t s8);
        void JP();
        void JP(ATwoByteRegister& reg);
        void JP_NZ();
        void JP_NC();
        void JP_Z();
        void JP_C();
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

        void ADD(AByteRegister& dst, const AByteRegister& src);
        void ADD(AByteRegister& dst, Address address);
        void ADD(AByteRegister& dst, uint8_t d8);
        void ADD(ATwoByteRegister& dst, int8_t s8);
        void ADD(ATwoByteRegister& dst, ATwoByteRegister& src);
        void ADC(AByteRegister& src);
        void ADC(Address address);
        void ADC(uint8_t d8);

        void SUB(AByteRegister& a);
        void SUB(Address address);
        void SUB(uint8_t d8);
        void SBC(AByteRegister& reg);
        void SBC(Address address);
        void SBC(uint8_t d8);

        void AND(AByteRegister& a);
        void AND(Address address);
        void AND(uint8_t d8);
        void OR(const AByteRegister& a);
        void OR(Address address);
        void OR(uint8_t d8);
        void XOR(const AByteRegister& reg);
        void XOR(Address address);
        void XOR(uint8_t d8);
        void CP(const AByteRegister& reg);
        void CP(Address address);
        void CP(uint8_t d8);

        void INC(ATwoByteRegister& reg);
        void INC(AByteRegister& reg);
        void INC(Address address);
        void DEC(ATwoByteRegister& reg);
        void DEC(AByteRegister& reg);
        void DEC(Address address);

        void CALL_NZ();
        void CALL_NC();
        void CALL_Z();
        void CALL_C();
        void CALL();

        void RET_NZ();
        void RET_NC();
        void RET_Z();
        void RET_C();
        void RET();
        void RETI();

        void POP(ATwoByteRegister& reg);
        void PUSH(const ATwoByteRegister& reg);

        uint8_t ReadA8();
        uint16_t ReadD16();
        int8_t ReadS8();
        uint16_t ReadA16();
        uint8_t ReadD8();

        CPU& m_cpu;
    };
}
