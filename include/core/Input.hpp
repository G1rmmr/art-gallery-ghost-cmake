#pragma once

#include <optional>
#include <unordered_map>

#include <SFML/Window/Event.hpp>

#include "Display.hpp"

namespace mir{
    namespace input{
        enum class Key{
            Escape, Space, Enter,
            W, A, S, D,
        };

        enum class Type{
            KeyPressed,
            KeyReleased,
            MousePressed,
            MouseReleased,
            WindowClosed
        };

        using KeyCallback = std::function<void(Key)>;
        using MouseCallback = std::function<void(int x, int y)>;
        using VoidCallback = std::function<void()>;

        namespace{
            static inline const std::unordered_map<Key, sf::Keyboard::Key> KeyMap = {
                {Key::Escape, sf::Keyboard::Key::Escape},
                {Key::W, sf::Keyboard::Key::W},
                {Key::A, sf::Keyboard::Key::A},
                {Key::S, sf::Keyboard::Key::S},
                {Key::D, sf::Keyboard::Key::D},
                {Key::Space, sf::Keyboard::Key::Space},
                {Key::Enter, sf::Keyboard::Key::Enter}
            };

            static inline const std::unordered_map<sf::Keyboard::Key, Key> ReverseKeyMap = {
                {sf::Keyboard::Key::Escape, Key::Escape},
                {sf::Keyboard::Key::W, Key::W},
                {sf::Keyboard::Key::A, Key::A},
                {sf::Keyboard::Key::S, Key::S},
                {sf::Keyboard::Key::D, Key::D},
                {sf::Keyboard::Key::Space, Key::Space},
                {sf::Keyboard::Key::Enter, Key::Enter}
            };

            static inline std::vector<KeyCallback> OnKeyPressedCallbacks;
            static inline std::vector<KeyCallback> OnKeyReleasedCallbacks;
            static inline std::vector<MouseCallback> OnMousePressedCallbacks;
            static inline std::vector<VoidCallback> OnWindowClosedCallbacks;
        }

        static inline bool IsPressed(Key key){
            return sf::Keyboard::isKeyPressed(KeyMap.at(key));
        }

        static inline void OnKeyPressed(KeyCallback callback){
            OnKeyPressedCallbacks.push_back(callback);
        }

        static inline void OnKeyReleased(KeyCallback callback){
            OnKeyReleasedCallbacks.push_back(callback);
        }

        static inline void OnMousePressed(MouseCallback callback){
            OnMousePressedCallbacks.push_back(callback);
        }

        static inline void OnWindowClosed(VoidCallback callback){
            OnWindowClosedCallbacks.push_back(callback);
        }

        static inline void Process(){
            while(const std::optional event = Window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    Window.close();

                    for(const VoidCallback& callback : OnWindowClosedCallbacks)
                        callback();
                }

                if(const sf::Event::KeyPressed* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                    if(ReverseKeyMap.contains(keyPressed->code)){
                        Key key = ReverseKeyMap.at(keyPressed->code);

                        for(const KeyCallback& callback : OnKeyPressedCallbacks)
                            callback(key);
                    }
                }

                if(const sf::Event::KeyReleased* keyReleased = event->getIf<sf::Event::KeyReleased>()){
                    if(ReverseKeyMap.contains(keyReleased->code)){
                        Key key = ReverseKeyMap.at(keyReleased->code);

                        for(const KeyCallback& callback : OnKeyReleasedCallbacks)
                            callback(key);
                    }
                }

                if(const sf::Event::MouseButtonPressed* mousePressed
                    = event->getIf<sf::Event::MouseButtonPressed>()){
                    for(const MouseCallback& callback : OnMousePressedCallbacks)
                        callback(mousePressed->position.x, mousePressed->position.y);
                }
            }
        }

        static inline void ClearAll(){
            OnKeyPressedCallbacks.clear();
            OnKeyReleasedCallbacks.clear();
            OnMousePressedCallbacks.clear();
            OnWindowClosedCallbacks.clear();
        }
    }
}
