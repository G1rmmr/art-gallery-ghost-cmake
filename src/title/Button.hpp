#pragma once

#include "Mir.hpp"
#include "handle/Event.hpp"
#include "handle/Input.hpp"
#include "util/Debugger.hpp"

namespace button{
    const std::string_view CONTENT = "Art Gallery Ghost";

    const float SIZE_X = 500.f;
    const float SIZE_Y = 50.f;

    const float POS_X = SIZE_X / 2.f;
    const float POS_Y = SIZE_Y / 2.f;

    namespace{
        inline void InitTransform(const ID id){
            transform::Positions[id] = {POS_X, POS_Y};
            transform::Scales[id] = {1.f, 1.f};
        }

        inline void InitPhysics(const ID id){
            physics::Bounds[id] = {SIZE_X, SIZE_Y};
        }

        inline void InitSprite(const ID id){
            sprite::Colors[id] = {0, 0, 0};
            sprite::Types[id] = sprite::Type::Rectangle;
            texture::AllocFromType(id);
        }

        inline void InitText(){
            const Tag tag = font::Create("fonts/dieproud.ttf");
            font::Alloc(tag);
            ui::BuildText(tag, {POS_X, POS_Y}, CONTENT.data(), 100);
        }

        inline void SubscribeOnClick(){
            debug::Log("CLICK");
        }

        inline void SubscribeMousePressed(){
            input::OnMousePressed([](int x, int y){
                if(static_cast<float>(x) < POS_X) return;
                if(static_cast<float>(x) > POS_X + SIZE_X) return;
                if(static_cast<float>(y) < POS_Y) return;
                if(static_cast<float>(y) > POS_Y + SIZE_Y) return;

                event::Publish(event::type::ButtonClick{});
            });
        }
    }

    inline ID Create(){
        const ID id = entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitPhysics(id);
        InitSprite(id);
        InitText();

        SubscribeOnClick();
        SubscribeTouch();

        return id;
    }
}
