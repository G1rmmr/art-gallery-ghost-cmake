#pragma once

#include "Mir.hpp"

#include "menu/Menu.hpp"
#include "game/Game.hpp"

const std::string_view WINDOW_TITLE = "art-gallery-ghost";
const std::string_view SAVE_PATH = "record.dat";
const std::uint8_t FPS = 60;

namespace app {
    namespace{
        inline void Initialize(){
            mir::window::Init(WINDOW_TITLE.data());
            mir::window::SetFPS(FPS);

            scene::Load(scene::Type::Menu);
        }

        inline void ProcessInput(){
            PROFILE_SCOPE("Input"){
                mir::input::Process();
                if(mir::input::IsPressed(mir::input::Key::Escape))
                    mir::window::Close();
            }
        }

        inline void Update(const float deltaTime){
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
            mir::DeleteAll();
            mir::window::Shutdown();
        }
    }

    inline void Run(){
        try{
            Initialize();
            while(mir::window::IsOpening()){
                const float deltaTime = mir::time::GetDelta();
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
