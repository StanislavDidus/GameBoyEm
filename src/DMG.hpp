#pragma once

#include <filesystem>

#include "CPU/CPU.hpp"
#include "MemoryBus/MemoryBus.hpp"

namespace dmg
{
    class DMG
    {
    public:
        DMG();
        ~DMG() = default;

        void LoadROM(const std::filesystem::path& path);
        void Update(double delta_time);
    private:
        MemoryBus m_memory_bus{};
        CPU m_cpu {m_memory_bus};

        double clock_timer = 0.0;
        double clock_time = 0.5;
    };
}