#pragma once

#include "Mir.hpp"

namespace game{
    const std::string_view WINDOW_TITLE = "art-gallery-ghost";
    const std::string_view SAVE_PATH = "record.dat";
    const std::uint8_t FPS = 60;

    void Initialize();
    void ProcessInput();
    void Update(const float deltaTime);
    void Render();
    void Exit();
}
