#pragma once

#include <cstdint>

#include "ControlUnit.hpp"
#include "MemoryBus/MemoryBus.hpp"
#include "Registers/ByteRegister.hpp"
#include "Registers/ByteRegisterPair.hpp"
#include "Registers/FlagRegister.hpp"
#include "Registers/TwoByteRegister.hpp"

namespace dmg
{
    class CPU
    {
    public:
        explicit CPU(MemoryBus& bus);

        void Decode();
    private:
        uint8_t Fetch();
        [[nodiscard]] uint8_t ReadMemory(uint16_t address) const;
        void WriteMemory(uint16_t address, uint8_t value);

        ByteRegister A, B, C, D, E, H, L;
        FlagRegister F;
        ByteRegisterPair AF, BC, DE, HL;

        TwoByteRegister PC {CARTRIDGE_HEADER};
        TwoByteRegister SP;
        //uint8_t IR = 0;
        //uint8_t IE = 0;

        ControlUnit control_unit {*this};

        MemoryBus& m_bus;

        friend class ControlUnit;
    };
}
