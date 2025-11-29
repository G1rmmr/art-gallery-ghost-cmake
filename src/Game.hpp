#pragma once

#include "Mir.hpp"

namespace game{
    constexpr const std::string_view WINDOW_TITLE = "art-gallery-ghost";
    constexpr const std::string_view SAVE_PATH = "record.dat";
    constexpr const std::uint8_t FPS = 60;

    static inline void Initialize();
    static inline void ProcessInput();
    static inline void Update(const float deltaTime);
    static inline void Render();
    static inline void Exit();

    static inline void Run(){
        Initialize();
        while(mir::window::IsOpening()){
            const float deltaTime = mir::time::GetDelta();
            ProcessInput();
            Update(deltaTime);
            Render();
        }
        Exit();
    }
}
