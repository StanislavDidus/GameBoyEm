#pragma once

#include <cstdint>

#include "ControlUnit.hpp"
#include "DMG/RegisterInfo.hpp"
#include "MemoryBus/MemoryBus.hpp"
#include "Registers/ByteRegister.hpp"
#include "Registers/ByteRegisterPair.hpp"
#include "Registers/FlagRegister.hpp"
#include "Registers/TwoByteRegister.hpp"
#include "Interrupt.hpp"

namespace dmg
{
    class CPU
    {
    public:
        explicit CPU(MemoryBus& bus);

        void SetRegisters(const RegisterInfo& register_info);

        uint32_t Step();
        void Reset();

        uint32_t HandleInterrupts();

        [[nodiscard]] RegisterInfo GetRegisterInfo() const;
    private:
        uint8_t Fetch();
        [[nodiscard]] uint8_t ReadMemory(uint16_t address) const;
        void WriteMemory(uint16_t address, uint8_t value);

        ByteRegister A, B, C, D, E, H, L;
        FlagRegister F;
        ByteRegisterPair AF, BC, DE, HL;

        TwoByteRegister PC {CARTRIDGE_HEADER};
        TwoByteRegister SP;

        bool IME = false;
        Interrupt IE {0};
        Interrupt IF {0};

        ControlUnit m_control_unit {*this};

        MemoryBus& m_bus;

        friend class ControlUnit;
    };
}
