#include "DMG/DMG.hpp"
#include "DMG/Utils/Log.hpp"
#include <fstream>

#include "MemoryBus/MemoryBus.hpp"
#include "CPU/CPU.hpp"

dmg::DMG::DMG()
{
    m_memory_bus = std::make_unique<MemoryBus>();
    m_cpu = std::make_unique<CPU>(*m_memory_bus);

    DMG_INFO("Game Boy was initialized.");
}

dmg::DMG::~DMG() = default;

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

    file.read(reinterpret_cast<char*>(m_memory_bus->GetDataPointer()), file_size);

    if (file.bad())
    {
        DMG_ERROR("Failed to read from a file: {}", path.string());
    }

    DMG_INFO("ROM loaded: {}", path.filename().string());
}

void dmg::DMG::StartFrame()
{
    cycle_target = T_CYCLES_PER_FRAME - cycle_accumulator;
    total_cycles = 0;
}

void dmg::DMG::Update()
{
    if (m_state == State::PLAY)
    {
        StartFrame();

        while (total_cycles < cycle_target)
        {
            total_cycles += m_cpu->Step();
        }

        EndFrame();
    }
    else if (m_state == State::PAUSE)
    {
        if (total_cycles >= cycle_target)
        {
            EndFrame();
            StartFrame();
        }
    }
}

void dmg::DMG::EndFrame()
{
    if (total_cycles > cycle_target)
    {
        cycle_accumulator = total_cycles - cycle_target;
    }
    else if (total_cycles == cycle_target)
    {
        cycle_accumulator = 0;
    }
}

void dmg::DMG::StepInstruction()
{
    total_cycles += m_cpu->Step();
}

void dmg::DMG::StepFrame()
{
    while (total_cycles < cycle_target)
    {
        total_cycles += m_cpu->Step();
    }
}

void dmg::DMG::StepCycles(uint32_t target_cycles)
{
    uint32_t current_cycles = 0;
    while (current_cycles < target_cycles)
    {
        current_cycles += m_cpu->Step();
    }
}

void dmg::DMG::SetState(State state)
{
    m_state = state;

    // Do actions when entering certain states
    switch (m_state)
    {
    case State::NONE:
        break;
    case State::IDLE:
        m_cpu->Reset();
        break;
    case State::PLAY:
        break;
    case State::PAUSE:
        break;
    default:
        break;
    }
}

void dmg::DMG::SetRegisters(const RegisterInfo& register_info)
{
    m_cpu->SetRegisters(register_info);
}

dmg::RegisterInfo dmg::DMG::GetRegisters() const
{
    return m_cpu->GetRegisterInfo();
}

uint8_t dmg::DMG::ReadFromMemory(uint16_t address)
{
    return m_memory_bus->Read(address);
}

void dmg::DMG::WriteToMemory(uint16_t address, uint8_t value)
{
    return m_memory_bus->Write(address, value);
}

uint8_t* dmg::DMG::GetMemoryPointer()
{
    return m_memory_bus->GetDataPointer();
}

uint32_t dmg::DMG::GetMemorySize()
{
    return m_memory_bus->GetMemorySize();
}

void dmg::DMG::ClearMemory()
{
    m_memory_bus->Clear();
}

