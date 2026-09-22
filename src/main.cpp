#include "DMG.hpp"
#include "Graphics/Graphics.hpp"
#include "Utils/Log.hpp"
#include "UI/UI.hpp"
#include "imgui_memory_editor.h"

int main()
{
    dmg::Log::Init();
    dmg::GraphicsContext graphics_context = dmg::InitializeGraphicsContext();
    dmg::InitializeImGui(graphics_context);

    dmg::DMG dmg = dmg::DMG{};
    dmg.LoadROM("roms/test_suits/03-op sp,hl.gb");

    double delta_time = 0.0;

    while (!glfwWindowShouldClose(graphics_context.window))
    {
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

        glfwPollEvents();

        int width, height;
        glfwGetFramebufferSize(graphics_context.window, &width, &height);
        glViewport(0, 0, width, height);

        dmg.Update(delta_time);

        glClear(GL_COLOR_BUFFER_BIT);

        dmg::StartImGuiFrame();

        ImGui::DockSpaceOverViewport();

        dmg::DMG::DMGInfo dmg_info = dmg.GetInfo();
        dmg::CPU::CPUInfo cpu_info = dmg_info.cpu_info;
        dmg::MemoryBus::MemoryInfo memory_info = dmg_info.memory_info;

        dmg::DrawRegisterWindow(cpu_info);
        dmg::DrawControlWindow(dmg, dmg_info);
        ImGui::ShowDemoWindow();
        static MemoryEditor mem_edit;
        mem_edit.HighlightMin = cpu_info.PC;
        mem_edit.HighlightMax = cpu_info.PC + 1;
        mem_edit.HighlightColor = IM_COL32(0, 255, 0, 255);
        mem_edit.DrawWindow("Memory", memory_info.data, memory_info.size);

        dmg::EndImGuiFrame();

        glfwSwapBuffers(graphics_context.window);

        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        using ms = std::chrono::duration<float, std::milli>;
        delta_time = std::chrono::duration_cast<ms>(end - start).count() / 1000.0;
    }

    dmg::DestroyImGui();
    dmg::DestroyGraphicsContext(graphics_context);

    DMG_INFO("Program finished.");

    return 0;
}
