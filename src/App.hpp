#pragma once

#include <Mir>

#include "game/Network.hpp"
#include "game/Player.hpp"
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
        mir::scene::Update();
    }

    inline void ProcessInput(){
        PROFILE_SCOPE("Input"){
            mir::input::Process();

            if(mir::input::IsPressed(mir::event::type::Key::Escape)){
                network::GetTopPlayers();

                mir::scene::Load("Menu");
            }
            if(mir::input::IsPressed(mir::event::type::Key::F1))
                mir::debug::ToggleDebug();

            if(mir::input::IsPressed(mir::event::type::Key::F2))
                mir::profile::ToggleProfile();
        }
    }

    inline void Update(const mir::Real deltaTime){
        mir::scene::Update();
        mir::time::Update(deltaTime);
        mir::profile::Update(deltaTime);
        mir::effect::Update(deltaTime);
        mir::movement::Update(deltaTime);
        mir::collision::Update();
        mir::animation::Update(deltaTime);
        mir::camera::Update(deltaTime);
        mir::combat::Update(deltaTime);
    }

    inline void Render(){
        PROFILE_SCOPE("Render"){
            mir::window::Clear(20, 20, 50);
            mir::window::Render();
            mir::window::Display();
        }
    }

    inline void Shutdown(){
        mir::Clear();
        mir::window::Shutdown();
    }
}
