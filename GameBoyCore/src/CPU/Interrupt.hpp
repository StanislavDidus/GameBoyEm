#pragma once
#include <cstdint>
#include <optional>

namespace dmg
{
    class Interrupt
    {
    public:
        Interrupt() = default;
        explicit Interrupt(uint8_t value) : m_value(value) {}
        ~Interrupt() = default;

        void Set(uint8_t value)
        {
            // NOTE: Mask value to make sure than the last 3 bits are not used in any way.
            value &= 0b00011111;
            m_value |= value;
        }

        void Reset(uint8_t value)
        {
            value &= 0b00011111;
            m_value &= ~value;
        }

        bool ReadBit(uint8_t i) const
        {
            if (i > 4 && i < 8) return false;
            return m_value >> i & 1;
        }

        std::optional<uint8_t> GetSharedBit(const Interrupt& interrupt)
        {
            // NOTE: We don't have to go through all 8 bits because
            // the last three are not used.
            for (int i = 0; i < 5; ++i)
            {
                if (ReadBit(i) && interrupt.ReadBit(i))
                {
                    return i;
                }
            }

            return std::nullopt;
        }

        static constexpr uint8_t VBLANK = 0b1;
        static constexpr uint8_t LCD    = 0b10;
        static constexpr uint8_t Timer  = 0b100;
        static constexpr uint8_t Serial = 0b1000;
        static constexpr uint8_t Joypad = 0b10000;
    private:
        uint8_t m_value = 0;
    };
}
