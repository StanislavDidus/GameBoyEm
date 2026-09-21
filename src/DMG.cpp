#include "DMG.hpp"
#include "Utils/Log.hpp"
#include <fstream>

dmg::DMG::DMG()
{
    DMG_INFO("Game Boy was initialized.");
}

void dmg::DMG::LoadROM(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        DMG_ERROR("Could not open rom file for reading: {}", path.string());
    }

    std::streamsize file_size = file.tellg();

    // TODO: Optionally check for the ROM size

    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char*>(m_memory_bus.GetMemoryPointer()), file_size);

    if (file.bad())
    {
        DMG_ERROR("Failed to read from a file: {}", path.string());
    }

    DMG_INFO("ROM loaded: {}", path.filename().string());
}

void dmg::DMG::Update(double delta_time)
{
    clock_timer += delta_time;
    if (clock_timer >= clock_time)
    {
        clock_timer = 0.0;

        m_cpu.Decode();
    }
}
