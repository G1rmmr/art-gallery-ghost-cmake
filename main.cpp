#include "src/App.hpp"

static inline void Run(){
    const mir::Real deltaTime = mir::time::GetDelta();
    app::ProcessInput();
    app::Update(deltaTime);
    app::Render();
}

int main() {
    try{
        app::Initialize();

        while(mir::window::IsOpening()) Run();
        app::Shutdown();
    }
    catch(const std::exception& e){
        mir::debug::Log("Exception: %s", e.what());
        app::Shutdown();
    }
    return 0;
}
