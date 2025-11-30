#pragma once

namespace mir{
    namespace scene{
        enum class Scene{ Menu, Game, Pause, GameOver };

        static inline void Load(Scene scene){
            return;
        }

        static inline Scene GetCurrent(){
            return Scene::GameOver;
        }
    }
}
