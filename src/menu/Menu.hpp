#pragma once

#include "Title.hpp"
#include "Button.hpp"

namespace menu{
    mir::ID TitleID = 0;
    mir::ID ButtonID = 0;

    inline void Initialize(){
        PROFILE_SCOPE("Initialization"){
            TitleID = title::Create();
            ButtonID = button::Create();
        }
        mir::debug::Log("Menu Initialized!");
    }
}
