#pragma once

#include <functional>
#include <unordered_map>

#include <SFML/Window.hpp>

#include "../view/Display.hpp"

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
            static inline std::vector<KeyCallback> OnKeyPressedCallbacks;
            static inline std::vector<KeyCallback> OnKeyReleasedCallbacks;
            static inline std::vector<MouseCallback> OnMousePressedCallbacks;
            static inline std::vector<MouseCallback> OnTouchCallbacks;
            static inline std::vector<VoidCallback> OnWindowClosedCallbacks;

            static inline const std::unordered_map<Key, sf::Keyboard::Key> KeyMap = {
                {Key::Escape, sf::Keyboard::Escape},
                {Key::W, sf::Keyboard::W},
                {Key::A, sf::Keyboard::A},
                {Key::S, sf::Keyboard::S},
                {Key::D, sf::Keyboard::D},
                {Key::Space, sf::Keyboard::Space},
                {Key::Enter, sf::Keyboard::Enter}
            };

            static inline const std::unordered_map<sf::Keyboard::Key, Key> ReverseKeyMap = {
                {sf::Keyboard::Escape, Key::Escape},
                {sf::Keyboard::W, Key::W},
                {sf::Keyboard::A, Key::A},
                {sf::Keyboard::S, Key::S},
                {sf::Keyboard::D, Key::D},
                {sf::Keyboard::Space, Key::Space},
                {sf::Keyboard::Enter, Key::Enter}
            };

            static inline void Process(){
                sf::Event event;
                while(Window->pollEvent(event)){
                    switch(event.type) {
                        case sf::Event::Closed:
                            Window->close();
                            for(const VoidCallback& callback : OnWindowClosedCallbacks)
                                callback();
                            break;
                        case sf::Event::KeyPressed:
                            if(ReverseKeyMap.contains(event.key.code)){
                                Key key = ReverseKeyMap.at(event.key.code);
                                for(const KeyCallback& callback : OnKeyPressedCallbacks)
                                    callback(key);
                            }
                            break;
                        case sf::Event::KeyReleased:
                            if(ReverseKeyMap.contains(event.key.code)){
                                Key key = ReverseKeyMap.at(event.key.code);
                                for(const KeyCallback& callback : OnKeyReleasedCallbacks)
                                    callback(key);
                            }
                            break;
                        case sf::Event::MouseButtonPressed:
                            for(const MouseCallback& callback : OnMousePressedCallbacks)
                                callback(event.mouseButton.x, event.mouseButton.y);
                            break;
                        case sf::Event::TouchBegan:
                            for(const MouseCallback& callback : OnTouchCallbacks)
                                callback(event.touch.x, event.touch.y);
                            break;
                        default:
                            break;
                    }
                }
            }
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

        static inline void OnTouchBegan(MouseCallback callback){
            OnTouchCallbacks.push_back(callback);
        }

        static inline void OnWindowClosed(VoidCallback callback){
            OnWindowClosedCallbacks.push_back(callback);
        }


        static inline void ClearAll(){
            OnKeyPressedCallbacks.clear();
            OnKeyReleasedCallbacks.clear();
            OnMousePressedCallbacks.clear();
            OnTouchCallbacks.clear();
            OnWindowClosedCallbacks.clear();
        }
    }
}
