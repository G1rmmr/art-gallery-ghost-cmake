#pragma once

#include <Mir>

namespace button{
    const mir::String CONTENT = "Art Gallery Ghost";

    const mir::Real SIZE_X = 500;
    const mir::Real SIZE_Y = 50;

    const mir::Real POS_X = SIZE_X / 2;
    const mir::Real POS_Y = SIZE_Y / 2;

    namespace{
        inline void InitTransform(const mir::ID id){
            mir::transform::Positions[id] = mir::Point2<mir::Real>(POS_X, POS_Y);
            mir::transform::Scales[id] = mir::Point2<mir::Real>(1, 1);
        }

        inline void InitPhysics(const mir::ID id){
            mir::physics::Bounds[id] = mir::Point2<mir::Real>(SIZE_X, SIZE_Y);
        }

        inline void InitSprite(const mir::ID id){
            mir::sprite::Colors[id] = mir::Color(0, 0, 0);
            mir::sprite::Types[id] = mir::sprite::Type::Rectangle;
            mir::texture::AllocFromType(id);
        }

        inline void InitText(){
            const mir::Tag tag = mir::font::Create("fonts/dieproud.ttf");
            mir::font::Alloc(tag);
            mir::ui::BuildText(
                tag,
                mir::Color(255, 255, 255),
                mir::Point2<mir::Real>(POS_X, POS_Y),
                CONTENT.data(),
                100);
        }

        inline void SubscribeOnClick(){
            mir::event::Subscribe(mir::Action<const mir::event::type::ButtonClick&>(
                [](const mir::event::type::ButtonClick&){
                    mir::scene::Load("Game");
                }
            ));
        }

        inline void SubscribeMousePressed(){
            mir::event::Subscribe(mir::Action<const mir::event::type::MousePressed&>(
                [](const mir::event::type::MousePressed& event){
                    if(static_cast<mir::Real>(event.X) < POS_X) return;
                    if(static_cast<mir::Real>(event.X) > POS_X + SIZE_X) return;
                    if(static_cast<mir::Real>(event.Y) < POS_Y) return;
                    if(static_cast<mir::Real>(event.Y) > POS_Y + SIZE_Y) return;

                    mir::event::Publish(mir::event::type::ButtonClick{});
                }
            ));
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

        return id;
    }
}
