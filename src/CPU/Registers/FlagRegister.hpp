#pragma once

#include "ByteRegister.hpp"

namespace dmg
{
    class FlagRegister final : public AByteRegister
    {
    public:
        FlagRegister() = default;
        ~FlagRegister() override = default;

        [[nodiscard]] uint8_t read() const override { return m_data; }
        void write(const uint8_t data) override{ m_data = data; }

        void set_z_flag(bool value) { m_data = value ? m_data | (1u << 7) : m_data & ~(1u << 7); }
        void set_n_flag(bool value) { m_data = value ? m_data | (1u << 6) : m_data & ~(1u << 6); }
        void set_h_flag(bool value) { m_data = value ? m_data | (1u << 5) : m_data & ~(1u << 5); }
        void set_c_flag(bool value) { m_data = value ? m_data | (1u << 4) : m_data & ~(1u << 4); }

        [[nodiscard]] uint8_t read_z_flag() const { return (m_data & 0b10000000) >> 7; }
        [[nodiscard]] uint8_t read_n_flag() const { return (m_data & 0b01000000) >> 6;}
        [[nodiscard]] uint8_t read_h_flag() const { return (m_data & 0b00100000) >> 5;}
        [[nodiscard]] uint8_t read_c_flag() const { return (m_data & 0b00010000) >> 4;}
    private:
        uint8_t m_data = 0;

    };
}
