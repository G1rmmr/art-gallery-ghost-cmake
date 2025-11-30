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
        inline void InitTransform(const ID id){
            transform::Positions[id] = {POS_X, POS_Y};
            transform::Scales[id] = {1.f, 1.f};
        }

        inline void InitPhysics(const ID id){
            physics::Bounds[id] = {SIZE_X, SIZE_Y};
            physics::Masses[id] = 10000.f;
            physics::InAirFlags[id] = IN_AIR;
            physics::IsGhosts[id] = IS_GHOST;
        }

        inline void InitSprite(const ID id){
            sprite::Colors[id] = {0, 0, 0};
            sprite::Types[id] = sprite::Type::Rectangle;
            texture::AllocFromType(id);
        }
    }

    inline ID Create(){
        const ID id = entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitPhysics(id);
        InitSprite(id);

        return id;
    }
}
