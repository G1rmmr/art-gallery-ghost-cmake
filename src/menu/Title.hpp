#pragma once

#include <Mir>

namespace title{
    const mir::String CONTENT = "Art Gallery Ghost";

    const mir::Real POS_X = 0.0;
    const mir::Real POS_Y = 0.0;

    const mir::Real SIZE_X = 1920.0;
    const mir::Real SIZE_Y = 1080.0;

    namespace{
        mir::ID TitleTag = 0;
        mir::ID AnounceTag = 0;

        inline void InitText(){
            TitleTag = mir::font::Create("fonts/dieproud.ttf");
            mir::font::Alloc(TitleTag);
            mir::ui::BuildText(
                TitleTag,
                mir::Color(255, 255, 255),
                mir::Point2<mir::Real>(POS_X / 2, POS_Y / 2),
                CONTENT,
                100);

            AnounceTag = mir::font::Create("fonts/dieproud.ttf");
            mir::font::Alloc(AnounceTag);
            mir::ui::BuildText(
                AnounceTag,
                mir::Color(255, 255, 255),
                mir::Point2<mir::Real>(POS_X / 2, POS_Y / 2 + 300),
                CONTENT,
                100);
        }

        inline void InitSound(){
            const mir::Tag tag = mir::sound::Create("sounds/thunder-bgm.mp3");
            mir::sound::AllocMusic(tag);
        }

        inline void SubscribeKeyPressed(const mir::ID id){
            mir::Action<const mir::event::type::KeyPressed&> action
                = [id](const mir::event::type::KeyPressed& event){
                mir::font::Delete(TitleTag);
                mir::font::Delete(AnounceTag);
            };

            mir::event::Subscribe(action);
        }
    }

    inline mir::ID Create(){
        const mir::ID id = mir::entity::Create();
        if(id == 0) return 0;

        InitText();
        InitSound();

        SubscribeKeyPressed(id);
        return id;
    }
}
