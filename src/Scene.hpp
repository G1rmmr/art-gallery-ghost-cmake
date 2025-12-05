#pragma once

#include "Mir.hpp"

namespace game { inline void Initialize(); }
namespace menu { inline void Initialize(); }

namespace scene{
    enum class Type{
        None,
        Menu,
        Game
    };

    inline Type Current = Type::None;

    inline void Load(Type next){
        if(Current == next) return;
        Current = next;

        mir::DeleteAll();

        switch(next){
        case Type::Menu:
            menu::Initialize();
            break;

        case Type::Game:
            game::Initialize();
            break;

        default:
            break;
        }
    }
}
