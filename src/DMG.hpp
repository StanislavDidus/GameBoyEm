#pragma once

#include "CPU/CPU.hpp"
#include "MemoryBus/MemoryBus.hpp"

namespace dmg
{
    class DMG
    {
    public:
        DMG();
        ~DMG() = default;
    private:
        MemoryBus m_memory_bus{};
        CPU m_cpu {m_memory_bus};
    };
}