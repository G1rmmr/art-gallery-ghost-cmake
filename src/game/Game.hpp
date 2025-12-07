#pragma once

#include "Player.hpp"
#include "Ground.hpp"
#include "TestParticle.hpp"

namespace game{
    mir::ID PlayerID = 0;
    mir::ID GroundID = 0;
    mir::ID TestParticleID = 0;

    inline void Initialize(){
        PROFILE_SCOPE("Initialization"){
            PlayerID = player::Create();
            GroundID = ground::Create();
            TestParticleID = particle::Create();
        }
        mir::debug::Log("Game Initialized!");
    }
}
