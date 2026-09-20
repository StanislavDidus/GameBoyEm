#include <iostream>

#include "DMG.hpp"
#include "Utils/Log.hpp"

int main()
{
    dmg::Log::Init();
    DMG_INFO("Logger was initialized.");

    dmg::DMG game_boy = dmg::DMG{};
    game_boy.LoadROM("roms/test_suits/03-op sp,hl.gb");
    game_boy.Start();

    DMG_INFO("Program finished.");

    return 0;
}
