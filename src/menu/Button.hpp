#pragma once

#include "Mir.hpp"

#include "Scene.hpp"

namespace button{
    const std::string_view CONTENT = "Art Gallery Ghost";

    const float SIZE_X = 500.f;
    const float SIZE_Y = 50.f;

    const float POS_X = SIZE_X / 2.f;
    const float POS_Y = SIZE_Y / 2.f;

    namespace{
        inline void InitTransform(const mir::ID id){
            mir::transform::Positions[id] = sf::Vector2f(POS_X, POS_Y);
            mir::transform::Scales[id] = sf::Vector2f(1.f, 1.f);
        }

        inline void InitPhysics(const mir::ID id){
            mir::physics::Bounds[id] = sf::Vector2f(SIZE_X, SIZE_Y);
        }

        inline void InitSprite(const mir::ID id){
            mir::sprite::Colors[id] = sf::Color(0, 0, 0);
            mir::sprite::Types[id] = mir::sprite::Type::Rectangle;
            mir::texture::AllocFromType(id);
        }

        inline void InitText(){
            const mir::Tag tag = mir::font::Create("fonts/dieproud.ttf");
            mir::font::Alloc(tag);
            mir::ui::BuildText(tag, sf::Vector2f(POS_X, POS_Y), CONTENT.data(), 100);
        }

        inline void SubscribeOnClick(){
            mir::event::Subscribe<mir::event::type::ButtonClick>(
                [](const mir::event::type::ButtonClick&){
                    scene::Load(scene::Type::Game);
                }
            );
        }

        inline void SubscribeMousePressed(){
            mir::input::OnMousePressed([](int x, int y){
                if(static_cast<float>(x) < POS_X) return;
                if(static_cast<float>(x) > POS_X + SIZE_X) return;
                if(static_cast<float>(y) < POS_Y) return;
                if(static_cast<float>(y) > POS_Y + SIZE_Y) return;

                mir::event::Publish(mir::event::type::ButtonClick{});
            });
        }

        inline void SubscribeKeyPressed(){
            mir::input::OnKeyPressed([](
                [[maybe_unused]] const mir::input::Key key){
                mir::event::Publish(mir::event::type::ButtonClick{});
            });
        }
    }

    inline mir::ID Create(){
        const mir::ID id = mir::entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitPhysics(id);
        InitSprite(id);
        InitText();

        SubscribeOnClick();
        SubscribeMousePressed();
        SubscribeKeyPressed();

        return id;
    }
}
