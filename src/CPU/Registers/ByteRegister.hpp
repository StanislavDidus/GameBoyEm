#pragma once
#include <cstdint>

#include "AByteRegister.hpp"

namespace dmg
{
    class ByteRegister final : public dmg::AByteRegister
    {
    public:
        ByteRegister() = default;
        ~ByteRegister() override = default;

        [[nodiscard]] uint8_t read() const override { return m_data; }
        void write(const uint8_t data) override{ m_data = data; }
    private:
        uint8_t m_data = 0;
    };
}