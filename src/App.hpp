#pragma once

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include <Mir>

#include "menu/Menu.hpp"
#include "game/Game.hpp"

const mir::String WINDOW_TITLE = "art-gallery-ghost";
const mir::String SAVE_PATH = "record.dat";
const mir::Uint FPS = 60;

namespace app {
    inline void Initialize(){
        mir::window::Init(WINDOW_TITLE);
        mir::window::SetFPS(FPS);

        mir::camera::Init();

        mir::scene::Register("Menu", menu::Initialize);
        mir::scene::Register("Game", game::Initialize);

        mir::scene::Load("Menu");
    }

    inline void ProcessInput(){
        PROFILE_SCOPE("Input"){
            mir::input::Process();

            if(mir::input::IsPressed(mir::event::type::Key::Escape)){
                
#ifdef __EMSCRIPTEN__
                emscripten_cancel_main_loop();
#endif
                mir::window::Close();
            }
            if(mir::input::IsPressed(mir::event::type::Key::F1))
                mir::debug::ToggleDebug();

            if(mir::input::IsPressed(mir::event::type::Key::F2))
                mir::profile::ToggleProfile();
        }
    }

    inline void Update(const mir::Real deltaTime){
        mir::time::Update(deltaTime);
        mir::profile::Update(deltaTime);
        mir::effect::Update(deltaTime);
        mir::movement::Update(deltaTime);
        mir::collision::Update();
        mir::animation::Update(deltaTime);
        mir::camera::Update(deltaTime);
    }

    inline void Render(){
        PROFILE_SCOPE("Render"){
            mir::window::Clear(127, 127, 127);
            mir::window::Render();
            mir::window::Display();
        }
    }

    inline void Shutdown(){
        mir::Clear();
        mir::window::Shutdown();
    }
}
