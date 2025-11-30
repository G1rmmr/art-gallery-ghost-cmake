#pragma once

#include <SFML/System.hpp>

namespace mir {
    namespace time{
        static inline sf::Clock Clock;
        static inline float GetDelta(){ return Clock.restart().asSeconds(); }

        inline static void After(float seconds, std::function<void()> callback){
            return;
        }

        inline static void Every(float interval, std::function<void()> callback){
            return;
        }
    }
}
