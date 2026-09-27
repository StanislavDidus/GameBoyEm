#pragma once

#include <filesystem>

#include "RegisterInfo.hpp"

namespace dmg
{
    constexpr uint32_t T_CYCLES_PER_SECOND = 4'194'304;
    constexpr uint32_t M_CYCLES_PER_SECOND = 1'048'576;
    constexpr uint32_t T_CYCLES_PER_FRAME = 70'221;
    constexpr uint32_t M_CYCLES_PER_FRAME = 17'555;
    constexpr float FRAMES_PER_SECOND = 59.73f;

    class CPU;
    class MemoryBus;

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

        /*struct DMGInfo
        {
            State dmg_state = State::NONE;
            CPU::CPUInfo cpu_info {};
            MemoryBus::MemoryInfo memory_info {};
        };*/

        DMG();
        ~DMG();

        void LoadROM(const std::filesystem::path& path);

        void Update();

        void StepInstruction();
        void StepFrame();
        void StepCycles(uint32_t target_cycles);

        void SetState(State state);

        // Setters and getters that are needed for objects outside of DMG class
        State GetState() { return m_state; }

        void SetRegisters(const RegisterInfo& register_info);
        [[nodiscard]] RegisterInfo GetRegisters() const;

        uint8_t ReadFromMemory(uint16_t address);
        void WriteToMemory(uint16_t address, uint8_t value);
        uint8_t* GetMemoryPointer();
        uint32_t GetMemorySize();
        void ClearMemory();
    private:
        void StartFrame();
        void EndFrame();

        std::unique_ptr<MemoryBus> m_memory_bus {};
        std::unique_ptr<CPU> m_cpu {};

        State m_state = State::NONE;

        uint32_t cycle_accumulator = 0;
        uint32_t cycle_target = T_CYCLES_PER_FRAME;
        uint32_t total_cycles = 0;
    };
}