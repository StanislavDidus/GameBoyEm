#pragma once
#include <cstdint>

#include "ATwoByteRegister.hpp"

namespace dmg
{
    class TwoByteRegister final : public ATwoByteRegister
    {
    public:
        TwoByteRegister() = default;
        explicit  TwoByteRegister(const uint16_t data)
            : m_data(data) {}
        ~TwoByteRegister() override = default;

        [[nodiscard]] uint8_t readLow() const override { return m_data & 0xFF; }
        [[nodiscard]] uint8_t readHigh() const override { return m_data >> 8; }
        [[nodiscard]] uint16_t readWord() const override { return m_data; }

        void writeLow(const uint8_t data) override { m_data = (readHigh() << 8) | data; }
        void writeHigh(const uint8_t data) override { m_data = readLow() | (data << 8); }
        void writeWord(const uint16_t data) override { m_data = data; }

        void increase(uint16_t value) { m_data += value; }

        //uint16_t increment() override { return m_data++; }
        //uint16_t decrement() override { return --m_data; }

        uint16_t operator++() override { return ++m_data; }
        uint16_t operator++(int) override { return m_data++; }
        uint16_t operator--() override { return --m_data; }
        uint16_t operator--(int) override { return m_data--; }

    private:
        uint16_t m_data = 0;
    };
}