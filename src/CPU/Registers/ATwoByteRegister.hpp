#pragma once
#include <cstdint>

namespace dmg
{
    class ATwoByteRegister
    {
    public:
        ATwoByteRegister() = default;
        virtual ~ATwoByteRegister() = default;

        [[nodiscard]] virtual uint8_t readLow() const = 0;
        [[nodiscard]] virtual uint8_t readHigh() const = 0;
        [[nodiscard]] virtual uint16_t readWord() const = 0;

        virtual void writeLow(uint8_t data) = 0;
        virtual void writeHigh(uint8_t data) = 0;
        virtual void writeWord(uint16_t data) = 0;

        //virtual uint16_t increment() = 0; // Post increment
        //virtual uint16_t decrement() = 0; // Pre decrement

        virtual uint16_t operator++() = 0;
        virtual uint16_t operator++(int) = 0;
        virtual uint16_t operator--() = 0;
        virtual uint16_t operator--(int) = 0;
    };
}