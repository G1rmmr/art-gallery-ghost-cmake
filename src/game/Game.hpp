#pragma once

#include "Mir.hpp"

#include "Player.hpp"
#include "Ground.hpp"

namespace game{
    mir::ID PlayerID = 0;
    mir::ID GroundID = 0;

    inline void Initialize(){
        PROFILE_SCOPE("Initialization"){
            PlayerID = player::Create();
            GroundID = ground::Create();
        }
        mir::debug::Log("Game Initialized!");
    }
}
