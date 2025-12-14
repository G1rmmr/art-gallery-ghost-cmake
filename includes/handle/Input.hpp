#pragma once

#include <SFML/Window.hpp>

#include <unordered_map>
#include <array>

#include "Event.hpp"
#include "../view/Display.hpp"
#include "../util/Types.hpp"

namespace mir{
    namespace input{
        namespace{
            static inline const Dictionary<sf::Keyboard::Key, event::type::Key> KeyMap = {
                {sf::Keyboard::Key::Escape, event::type::Key::Escape},
                {sf::Keyboard::Key::W, event::type::Key::W},
                {sf::Keyboard::Key::A, event::type::Key::A},
                {sf::Keyboard::Key::S, event::type::Key::S},
                {sf::Keyboard::Key::D, event::type::Key::D},
                {sf::Keyboard::Key::F1, event::type::Key::F1},
                {sf::Keyboard::Key::F2, event::type::Key::F2},
                {sf::Keyboard::Key::Space, event::type::Key::Space},
                {sf::Keyboard::Key::Enter, event::type::Key::Enter}
            };

            static inline Array<Bool, static_cast<size_t>(sf::Keyboard::KeyCount)> IsPressedState;
        }

        static inline Bool IsPressed(event::type::Key key){
            for(const auto& [input, value] : KeyMap){
                if(value == key) return IsPressedState[static_cast<size_t>(input)];
            }
            return false;
        }

        static inline void Process(){
            while(const auto event = Window->pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    event::Publish(event::type::Death{});
                }
                else if(const auto* key = event->getIf<sf::Event::KeyPressed>()){
                    size_t code = static_cast<size_t>(key->code);
                    if(KeyMap.count(key->code) && !IsPressedState[code]){
                        IsPressedState[code] = true;
                        event::Publish(event::type::KeyPressed{KeyMap.at(key->code)});
                    }
                }
                else if(const auto* key = event->getIf<sf::Event::KeyReleased>()){
                     size_t code = static_cast<size_t>(key->code);
                     if(KeyMap.count(key->code)){
                        IsPressedState[code] = false;
                        event::Publish(event::type::KeyReleased{KeyMap.at(key->code)});
                    }
                }
                else if(const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()){
                    event::Publish(event::type::MousePressed{mouse->position.x, mouse->position.y});
                }
                else if(const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()){
                    event::Publish(event::type::MouseReleased{mouse->position.x, mouse->position.y});
                }
            }
        }
    }
}