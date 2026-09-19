#pragma once

#include "ATwoByteRegister.hpp"
#include "AByteRegister.hpp"

namespace dmg
{
    class ByteRegisterPair final : public ATwoByteRegister
    {
    public:
        ByteRegisterPair(AByteRegister& high, AByteRegister& low)
            : m_high(high)
            , m_low(low) {}
        ~ByteRegisterPair() override = default;

        [[nodiscard]] uint8_t readLow() const override { return m_low.read(); }
        [[nodiscard]] uint8_t readHigh() const override { return m_high.read(); }
        [[nodiscard]] uint16_t readWord() const override { return (m_high.read() << 8) | m_low.read(); }

        void writeLow(const uint8_t data) override { m_low.write(data); }
        void writeHigh(const uint8_t data) override { m_high.write(data); }
        void writeWord(const uint16_t data) override { m_high.write(data >> 8); m_low.write(data & 0xFF); }

        //uint16_t increment() override { uint16_t temp = readWord(); writeWord(readWord() + 1); return temp; }
        //uint16_t decrement() override { writeWord(readWord() - 1); return readWord();}

        uint16_t operator++()
        {
            writeWord(readWord() + 1);
            return readWord();
        }

        uint16_t operator++(int)
        {
            uint16_t temp = readWord();
            writeWord(readWord() + 1);
            return temp;
        }

        uint16_t operator--()
        {
            writeWord(readWord() - 1);
            return readWord();
        }

        uint16_t operator--(int)
        {
            uint16_t temp = readWord();
            writeWord(readWord() - 1);
            return temp;
        }
    private:
        AByteRegister& m_high;
        AByteRegister& m_low;
    };
}