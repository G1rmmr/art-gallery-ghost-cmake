#include "Game.hpp"

void game::Initialize(){
    PROFILE_SCOPE("Initialization"){
        mir::window::Init(WINDOW_TITLE.data());
        mir::window::SetFPS(FPS);
        mir::record::LoadAll(SAVE_PATH.data());
    }
    mir::debug::Log("Game Initialized!");
}

void game::ProcessInput(){
    PROFILE_SCOPE("Input"){ mir::input::Process(); }

    if(mir::input::IsPressed(mir::input::Key::Escape)){
        mir::window::Close();
    }
}

void game::Update(const float deltaTime){
    PROFILE_SCOPE("Movement"){ mir::movement::Update(deltaTime); }
    PROFILE_SCOPE("Collision"){ mir::collision::Update(); }
    PROFILE_SCOPE("RenderUpdate"){ mir::render::Update(deltaTime); }
}

void game::Render(){
    PROFILE_SCOPE("Render"){
        mir::window::Clear(0, 0, 0);
        mir::window::Render();
        mir::window::Display();
    }
}

void game::Exit(){
    PROFILE_SCOPE("Shutdown"){
        mir::record::SaveAll(game::SAVE_PATH.data());
        mir::input::ClearAll();
    }
    mir::debug::Log("Game Over!");
}
