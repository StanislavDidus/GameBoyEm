#pragma once

#include <filesystem>

#include "CPU/CPU.hpp"
#include "MemoryBus/MemoryBus.hpp"

namespace dmg
{
    class DMG
    {
    public:
        enum class State
        {
            NONE,
            IDLE,
            PLAY,
            PAUSE,
        };

        struct DMGInfo
        {
            State dmg_state = State::NONE;
            CPU::CPUInfo cpu_info {};
            MemoryBus::MemoryInfo memory_info {};
        };

        DMG();
        ~DMG() = default;

        void LoadROM(const std::filesystem::path& path);
        void Update(double delta_time);

        void SetState(State state);

        DMGInfo GetInfo() const;
    private:
        MemoryBus m_memory_bus{};
        CPU m_cpu {m_memory_bus};

        State m_state = State::NONE;

        double clock_timer = 0.0;
        double clock_time = 0.5;
    };
}