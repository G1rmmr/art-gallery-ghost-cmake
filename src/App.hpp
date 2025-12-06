#pragma once

#include "Mir.hpp"

#include "menu/Menu.hpp"
#include "game/Game.hpp"

const mir::String WINDOW_TITLE = "art-gallery-ghost";
const mir::String SAVE_PATH = "record.dat";
const mir::Uint FPS = 60;

namespace app {
    namespace{
        inline void Initialize(){
            mir::window::Init(WINDOW_TITLE);
            mir::window::SetFPS(FPS);

            mir::scene::Register("Menu", menu::Initialize);
            mir::scene::Register("Game", game::Initialize);

            mir::scene::Load("Menu");
        }

        inline void ProcessInput(){
            PROFILE_SCOPE("Input"){
                mir::input::Process();
                if(mir::input::IsPressed(mir::event::type::Key::Escape))
                    mir::window::Close();
            }
        }

        inline void Update(const mir::Real deltaTime){
            mir::movement::Update(deltaTime);
            mir::collision::Update();
            mir::animation::Update(deltaTime);
        }

        inline void Render(){
            PROFILE_SCOPE("Render"){
                mir::window::Clear(0x7F, 0x7F, 0x7F);
                mir::window::Render();
                mir::window::Display();
            }
        }

        inline void Shutdown(){
            mir::Clear();
            mir::window::Shutdown();
        }
    }

    inline void Run(){
        try{
            Initialize();
            while(mir::window::IsOpening()){
                const mir::Real deltaTime = mir::time::GetDelta();
                ProcessInput();
                Update(deltaTime);
                Render();
            }
            Shutdown();
        }
        catch(const std::exception& e){
            mir::debug::Log("Exception: %s", e.what());
            Shutdown();
        }
    }
}
