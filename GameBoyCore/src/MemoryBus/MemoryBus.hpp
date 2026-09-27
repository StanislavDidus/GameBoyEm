#pragma once
#include <cstdint>
#include <vector>

#include "../../include/DMG/DMGInitState.hpp"

namespace dmg
{
    // Memory mappings
    static constexpr uint16_t ROM_BANK_0 = 0;
    static constexpr uint16_t CARTRIDGE_HEADER = 0x100;
    static constexpr uint16_t ROM_BANK_N = 0x4000;
    static constexpr uint16_t VIDEO_RAM = 0x8000;
    static constexpr uint16_t EXTERNAL_RAM = 0xA000;
    static constexpr uint16_t WORK_RAM_BANK_0 = 0xC000;
    static constexpr uint16_t WORK_RAM_BANK_N = 0xD000;
    static constexpr uint16_t ECHO_RAM = 0xE000;
    static constexpr uint16_t OAM = 0xFE00;
    static constexpr uint16_t NOT_USABLE = 0xFEA0;
    static constexpr uint16_t IO_REGISTERS = 0xFF00;
    static constexpr uint16_t HIGH_RAM = 0xFF80;
    static constexpr uint16_t IE = 0xFFFF;

    class MemoryBus
    {
    public:
        /*
        struct MemoryInfo
        {
            uint8_t* data = nullptr;
            size_t size = 65'536;
        };
        */

        MemoryBus() = default;
        ~MemoryBus() = default;

        /*void InitializeWith(const std::vector<MemoryMapping>& memory_mappings)
        {
            for (const auto& [address, value] : memory_mappings)
            {
                m_data[address] = value;
            }
        }*/

        [[nodiscard]] uint16_t Read(uint16_t address) const { return m_data[address]; }
        void Write(uint16_t address, uint8_t value) { m_data[address] = value; }

        uint8_t* GetDataPointer() { return m_data; }
        uint32_t GetMemorySize() { return 65'536; }

        void Clear() { memset(m_data, 0, sizeof(m_data)); }
    private:
        mutable uint8_t m_data[65'536]; // 64 KiB
    };
}
