#pragma once

#include "Mir.hpp"

namespace title{
    const mir::String CONTENT = "Art Gallery Ghost";

    const mir::Real POS_X = 0.0;
    const mir::Real POS_Y = 0.0;

    const mir::Real SIZE_X = 1920.0;
    const mir::Real SIZE_Y = 1080.0;

    namespace{
        inline void InitTransform(const mir::ID id){
            mir::transform::Positions[id] = mir::Point2<mir::Real>(POS_X, POS_Y);
            mir::transform::Scales[id] = mir::Point2<mir::Real>(1.0, 1.0);
        }

        inline void InitSprite(const mir::ID id){
            mir::sprite::Colors[id] = mir::Color(0xFF, 0xFF, 0xFF);
            mir::sprite::Types[id] = mir::sprite::Type::None;
            mir::sprite::Sizes[id] = mir::Point2<mir::Real>(SIZE_X, SIZE_Y);

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
