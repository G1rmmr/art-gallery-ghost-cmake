#pragma once

#include "Mir.hpp"

namespace title{
    const std::string_view CONTENT = "Art Gallery Ghost";

    const float POS_X = 0.f;
    const float POS_Y = 0.f;

    const float SIZE_X = 1920.f;
    const float SIZE_Y = 1080.f;

    namespace{
        inline void InitTransform(const mir::ID id){
            mir::transform::Positions[id] = {POS_X, POS_Y};
            mir::transform::Scales[id] = {1.f, 1.f};
        }

        inline void InitSprite(const mir::ID id){
            mir::sprite::Colors[id] = {0xFF, 0xFF, 0xFF};
            mir::sprite::Types[id] = mir::sprite::Type::None;
            mir::sprite::Sizes[id] = {SIZE_X, SIZE_Y};

            const mir::Tag tag = mir::texture::Create("textures/title-image.jpg");
            mir::texture::AllocFromFile(id, tag);
        }

        inline void InitSound(){
            const mir::Tag tag = mir::sound::Create("sounds/thunder-bgm.mp3");
            mir::sound::AllocMusic(tag);
        }
    }

    inline mir::ID Create(){
        const mir::ID id = mir::entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitSprite(id);
        InitSound();

        return id;
    }
}
