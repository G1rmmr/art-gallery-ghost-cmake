#pragma once

#include <optional>

#include "Manager.hpp"

namespace mir{
    namespace input{
        static inline void Process(){
            while(const std::optional event = Window.pollEvent()){
                if(event->is<sf::Event::Closed>()) Window.close();

                if(const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                    if(keyPressed->code == sf::Keyboard::Key::Escape) Window.close();
                }
            }
        }
    }
}
