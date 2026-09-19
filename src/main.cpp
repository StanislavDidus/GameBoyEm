#include <iostream>

#include "DMG.hpp"
#include "Utils/Log.hpp"

int main()
{
    dmg::Log::Init();
    DMG_INFO("Logger was initialized.");

    dmg::DMG game_boy = dmg::DMG{};

    DMG_INFO("Program finished.");

    return 0;
}
