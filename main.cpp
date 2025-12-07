#include "includes/view/Display.hpp"
#include "src/App.hpp"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif

static inline void Run(){
    const mir::Real deltaTime = mir::time::GetDelta();
    app::ProcessInput();
    app::Update(deltaTime);
    app::Render();
}

int main() {
    try{
        app::Initialize();

#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(Run, 0, 1);
#else
        while(mir::window::IsOpening()) Run();
        app::Shutdown();
#endif
    }
    catch(const std::exception& e){
        mir::debug::Log("Exception: %s", e.what());

#ifndef __EMSCRIPTEN__
        app::Shutdown();
#endif

    }
    return 0;
}
