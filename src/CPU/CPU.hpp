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
    private:
        uint8_t fetch();
        uint8_t read_memory(uint16_t address) const;
        void write_memory(uint16_t address, uint8_t value);

        ByteRegister A, B, C, D, E, H, L;
        FlagRegister F;
        ByteRegisterPair AF, BC, DE, HL;

        TwoByteRegister PC;
        TwoByteRegister SP;
        //uint8_t IR = 0;
        //uint8_t IE = 0;

        ControlUnit control_unit {*this};

        MemoryBus& m_bus;

        friend class ControlUnit;
    };
}
