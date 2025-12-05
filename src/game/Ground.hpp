#pragma once

#include "Mir.hpp"

namespace ground{
    const float POS_X = -100.f;
    const float POS_Y = 700.f;

    const int SIZE_X = 2000;
    const int SIZE_Y = 200;

    const bool IN_AIR = false;
    const bool IS_GHOST = false;

    namespace{
        inline void InitTransform(const mir::ID id){
            mir::transform::Positions[id] = sf::Vector2f(POS_X, POS_Y);
            mir::transform::Scales[id] = sf::Vector2f(1.f, 1.f);
        }

        inline void InitPhysics(const mir::ID id){
            mir::physics::Bounds[id] = sf::Vector2f(SIZE_X, SIZE_Y);
            mir::physics::Masses[id] = 10000.f;
            mir::physics::InAirFlags[id] = IN_AIR;
            mir::physics::IsGhosts[id] = IS_GHOST;
        }

        inline void InitSprite(const mir::ID id){
            mir::sprite::Colors[id] = sf::Color(0, 0, 0);
            mir::sprite::Types[id] = mir::sprite::Type::Rectangle;
            mir::texture::AllocFromType(id);
        }
    }

    inline mir::ID Create(){
        const mir::ID id = mir::entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitPhysics(id);
        InitSprite(id);

        return id;
    }
}
