#include "DMG.hpp"
#include "Graphics/Graphics.hpp"
#include "Utils/Log.hpp"
#include "UI/UI.hpp"

int main()
{
    dmg::Log::Init();

    dmg::GraphicsContext graphics_context = dmg::InitializeGraphicsContext();
    dmg::InitializeImGui(graphics_context);

    dmg::DMG game_boy = dmg::DMG{};
    game_boy.LoadROM("roms/test_suits/03-op sp,hl.gb");

    double delta_time = 0.0;

    while (!glfwWindowShouldClose(graphics_context.window))
    {
        std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

        game_boy.Update(delta_time);

        glfwPollEvents();

        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        using ms = std::chrono::duration<float, std::milli>;
        delta_time = std::chrono::duration_cast<ms>(end - start).count() / 1000.0;
    }

    dmg::DestroyGraphicsContext(graphics_context);

    DMG_INFO("Program finished.");

    return 0;
}
