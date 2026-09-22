#pragma once

#include "DMG.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "CPU/CPU.hpp"

#include "Graphics/Graphics.hpp"

namespace dmg
{
    inline void InitializeImGui(const GraphicsContext& graphics_context)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(graphics_context.window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();

        io.FontGlobalScale = 2.0f;

        DMG_INFO("ImGui UI was initialized.");
    }

    inline void StartImGuiFrame()
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    inline void EndImGuiFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    inline void DestroyImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        DMG_INFO("ImGui UI was destroyed.");
    }

    template<typename T>
    void PrintRegisterValue(const std::string& text, T value)
    {
        ImGui::Text(text.c_str());
        ImGui::SameLine();
        ImGui::Text("%X", value);
    }

    inline void DrawRegisterWindow(const CPU::CPUInfo& cpu_info)
    {
        if (ImGui::Begin("Registers"))
        {
            if (ImGui::BeginTable("##anything", 2))
            {
                ImGui::TableNextColumn();
                PrintRegisterValue("PC: ", cpu_info.PC);
                PrintRegisterValue("A: ", cpu_info.A);
                PrintRegisterValue("B: ", cpu_info.B);
                PrintRegisterValue("D: ", cpu_info.D);
                PrintRegisterValue("H: ", cpu_info.H);
                ImGui::TableNextColumn();
                PrintRegisterValue("SP: ", cpu_info.SP);
                PrintRegisterValue("F: ", cpu_info.F);
                PrintRegisterValue("C: ", cpu_info.C);
                PrintRegisterValue("E: ", cpu_info.E);
                PrintRegisterValue("L: ", cpu_info.L);
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }

    inline void DrawControlWindow(DMG& dmg, const DMG::DMGInfo& dmg_info)
    {
        if (ImGui::Begin("Control"))
        {
            if (ImGui::Button("Start"))
            {
                dmg.SetState(DMG::State::PLAY);
            }

            if (dmg_info.dmg_state != DMG::State::PLAY) ImGui::BeginDisabled();

            ImGui::SameLine();

            if (ImGui::Button("Pause"))
            {
                dmg.SetState(DMG::State::PAUSE);
            }

            if (dmg_info.dmg_state != DMG::State::PLAY) ImGui::EndDisabled();

            if (ImGui::Button("Start & Pause"))
            {
                dmg.SetState(DMG::State::PAUSE);
            }

            if (dmg_info.dmg_state != DMG::State::PLAY) ImGui::BeginDisabled();

            if (ImGui::Button("Stop"))
            {
                dmg.SetState(DMG::State::IDLE);
            }

            if (dmg_info.dmg_state != DMG::State::PLAY) ImGui::EndDisabled();

            if (dmg_info.dmg_state != DMG::State::PAUSE) ImGui::BeginDisabled();

            ImGui::SameLine();

            if (ImGui::Button("Step"))
            {
            }

            if (dmg_info.dmg_state != DMG::State::PAUSE) ImGui::EndDisabled();
        }
        ImGui::End();
    }
}