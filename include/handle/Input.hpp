#pragma once

#include <SFML/Window.hpp>

#include <unordered_map>
#include <array>

#include "Event.hpp"
#include "../view/Display.hpp"

namespace mir{
    namespace input{
        namespace{
            static inline const std::unordered_map<sf::Keyboard::Key, event::type::Key> KeyMap = {
                {sf::Keyboard::Escape, event::type::Key::Escape},
                {sf::Keyboard::W, event::type::Key::W},
                {sf::Keyboard::A, event::type::Key::A},
                {sf::Keyboard::S, event::type::Key::S},
                {sf::Keyboard::D, event::type::Key::D},
                {sf::Keyboard::Space, event::type::Key::Space},
                {sf::Keyboard::Enter, event::type::Key::Enter}
            };

            static inline std::array<bool, sf::Keyboard::KeyCount> IsPressedState;
        }

        static inline bool IsPressed(event::type::Key key){
            for(const auto& [input, value] : KeyMap){
                if(value == key) return IsPressedState[input];
            }
            return false;
        }

        static inline void Process(){
            sf::Event event;

            while(Window->pollEvent(event)){
                std::size_t code = static_cast<std::size_t>(event.key.code);

                switch(event.type){
                    case sf::Event::Closed:
                        event::Publish(event::type::Death{});
                        break;

                    case sf::Event::KeyPressed:
                        if(KeyMap.count(event.key.code) && !IsPressedState[code]){
                            IsPressedState[code] = true;
                            event::Publish(event::type::KeyPressed{KeyMap.at(event.key.code)});
                        }
                        break;

                    case sf::Event::KeyReleased:
                        if (KeyMap.count(event.key.code)){
                            IsPressedState[code] = false;
                            event::Publish(event::type::KeyReleased{KeyMap.at(event.key.code)});
                        }
                        break;

                    case sf::Event::MouseButtonPressed:
                        event::Publish(event::type::MousePressed{event.mouseButton.x, event.mouseButton.y});
                        break;

                    case sf::Event::MouseButtonReleased:
                        event::Publish(event::type::MouseReleased{event.mouseButton.x, event.mouseButton.y});
                        break;

                    default: break;
                }
            }
        }
    }
}
