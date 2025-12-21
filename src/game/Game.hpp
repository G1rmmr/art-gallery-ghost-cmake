#pragma once

#include "Player.hpp"
#include "Ground.hpp"

namespace game{
    mir::ID PlayerID = 0;
    mir::ID GroundID = 0;

    inline void Initialize(){
        PROFILE_SCOPE("Initialization"){
            mir::camera::Init();

            PlayerID = player::Create();
            GroundID = ground::Create();

            mir::camera::Follow(PlayerID);
        }
        mir::debug::Log("Game Initialized!");
    }
}
