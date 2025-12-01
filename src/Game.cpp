#include "Game.hpp"

#include "main/Player.hpp"
#include "main/Ground.hpp"

#include "title/Title.hpp"

ID PlayerID = 0;
ID GroundID = 0;

ID TitleID = 0;

void game::Initialize(){
    PROFILE_SCOPE("Initialization"){
        window::Init(WINDOW_TITLE.data());
        window::SetFPS(FPS);

        TitleID = title::Create();

        // PlayerID = player::Create();
        // GroundID = ground::Create();

        // mir::record::LoadAll(SAVE_PATH.data());
    }
    debug::Log("Game Initialized!");
}

void game::ProcessInput(){
    PROFILE_SCOPE("Input"){
        input::Process();

        if(input::IsPressed(input::Key::Escape))
            window::Close();
    }
}

void game::Update(const float deltaTime){
    movement::Update(deltaTime);
    collision::Update();
    render::Update(deltaTime);
}

void game::Render(){
    PROFILE_SCOPE("Render"){
        window::Clear(0x7F, 0x7F, 0x7F);
        window::Render();
        window::Display();
    }
}

void game::Exit(){
    PROFILE_SCOPE("Shutdown"){
        // mir::record::SaveAll(game::SAVE_PATH.data());
        input::ClearAll();
        event::ClearAll();

        texture::DeleteAll();
        sound::DeleteAll();
        font::DeleteAll();

        window::Shutdown();
    }
    debug::Log("Game Over!");
}
