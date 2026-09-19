#pragma once
#include <cstdint>

namespace dmg
{
    class AByteRegister
    {
    public:
        AByteRegister() = default;
        virtual ~AByteRegister() = 0;

        [[nodiscard]] virtual uint8_t read() const = 0;
        virtual void write(uint8_t data) = 0;
    };

    inline AByteRegister::~AByteRegister()
    {
    }
}
