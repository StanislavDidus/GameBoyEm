#include "DMG.hpp"
#include "Utils/Log.hpp"
#include <fstream>

dmg::DMG::DMG()
{
    DMG_INFO("Game Boy was successfully created.");
}

void dmg::DMG::LoadROM(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        DMG_ERROR("Could not open rom file for reading: {}\n", path.string());
    }

    std::streamsize file_size = file.tellg();

    // TODO: Optionally check for the ROM size

    file.seekg(0, std::ios::beg);

    file.read(reinterpret_cast<char*>(m_memory_bus.GetMemoryPointer()), file_size);

    if (file.bad())
    {
        DMG_ERROR("Failed to read from a file: {}\n", path.string());
    }

    DMG_INFO("Successfully loaded ROM: {}\n", path.filename().string());
}

void dmg::DMG::Start()
{
    bool is_running = true;

    double delta = 0.0;
    while (is_running)
    {
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

        clock_timer += delta;
        if (clock_timer >= clock_time)
        {
            clock_timer = 0.0;
            m_cpu.Decode();
        }

        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        using ms = std::chrono::duration<float, std::milli>;
        delta = std::chrono::duration_cast<ms>(end - start).count() / 1000.0;
    }
}
