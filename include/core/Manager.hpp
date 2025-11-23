#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <string_view>

#include <SFML/Graphics.hpp>

namespace mir {
    using ID = uint16_t;

    class Manager {
    public:
        sf::RenderWindow Window;

        Manager(const Manager&) = delete;
       	Manager& operator=(const Manager&) = delete;

        Manager(Manager&&) = delete;
        Manager& operator=(Manager&&) = delete;

        ~Manager() = default;

        static Manager& GetInstance() {
            static Manager instance;
            return instance;
        }

        static void ProcessEvent(sf::RenderWindow& window){
            while(const std::optional event = window.pollEvent()){
                if(event->is<sf::Event::Closed>()) window.close();

                if(const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                    if(keyPressed->code == sf::Keyboard::Key::Escape) window.close();
                }
            }
        }

    private:
        Manager() = default;
    };
}
