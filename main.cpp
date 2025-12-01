#include "src/Game.hpp"

int main() {
    try{
        game::Initialize();

        while(window::IsOpening()){
            const float deltaTime = time::GetDelta();
            game::ProcessInput();
            game::Update(deltaTime);
            game::Render();
        }
        game::Exit();
    }
    catch(const std::exception& e){
        debug::Log("Exception: %s", e.what());
        game::Exit();
    }
    return 0;
}
