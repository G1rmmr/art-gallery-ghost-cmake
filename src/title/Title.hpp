#pragma once

#include "Mir.hpp"

namespace title{
    const std::string_view CONTENT = "Art Gallery Ghost";

    const float POS_X = 0.f;
    const float POS_Y = 0.f;

    const float SIZE_X = 1920.f;
    const float SIZE_Y = 1080.f;

    namespace{
        inline void InitTransform(const ID id){
            transform::Positions[id] = {POS_X, POS_Y};
            transform::Scales[id] = {1.f, 1.f};
        }

        inline void InitSprite(const ID id){
            sprite::Colors[id] = {0xFF, 0xFF, 0xFF};
            sprite::Types[id] = sprite::Type::None;
            sprite::Sizes[id] = {SIZE_X, SIZE_Y};

            const Tag tag = texture::Create("textures/title-image.jpg");
            texture::AllocFromFile(id, tag);
        }

        inline void InitSound(){
            const Tag tag = sound::Create("sounds/thunder-bgm.mp3");
            sound::AllocMusic(tag);
        }
    }

    inline ID Create(){
        const ID id = entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitSprite(id);
        InitSound();

        return id;
    }
}
