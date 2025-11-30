#pragma once

#include <SFML/Window/WindowEnums.hpp>
#include <memory>

#include "../core/Entity.hpp"
#include "../core/Components.hpp"
#include "../util/Debugger.hpp"

namespace mir{
    inline sf::RenderWindow* Window = nullptr;

    enum class VideoMode{
        Windowed,
        Fullscreen,
        Borderless,
        Desktop
    };

    enum class Resolution{
        HD,  // 1280x720
        FHD, // 1920x1080
        QHD, // 2560x1440
        UHD, // 3840x2160
        Custom
    };

    namespace window{
        static inline bool IsOpening() { return Window && Window->isOpen();}

        static inline void Display(){
            if(Window) Window->display();
        }

        static inline void SetFPS(const std::uint8_t fps){
            if(Window) Window->setFramerateLimit(fps);
        }

        static inline void Close(){
            if(Window) Window->close();
        }

        static inline void Clear(std::uint8_t red, std::uint8_t green, std::uint8_t blue){
            if(Window) Window->clear(sf::Color(red, green, blue));
        }

        static inline void Shutdown(){
            if(Window){
                Window->close();
                delete Window;
            }
        }

        static inline void Init(
            const std::string& title,
            VideoMode mode = VideoMode::Desktop,
            Resolution res = Resolution::FHD,
            uint32_t width = 0,
            uint32_t height = 0){
            if(Window) return;
            sf::VideoMode video;

            switch(res){
            case Resolution::HD:
                video = sf::VideoMode({1280, 720});
                break;

            case Resolution::FHD:
                video = sf::VideoMode({1920, 1080});
                break;

            case Resolution::QHD:
                video = sf::VideoMode({2560, 1440});
                break;

            case Resolution::UHD:
                video = sf::VideoMode({3840, 2160});
                break;

            case Resolution::Custom:
                video = sf::VideoMode({width, height});
                break;

            default:
                video = sf::VideoMode::getDesktopMode();
                break;
            }

            switch(mode) {
            case VideoMode::Borderless:
                video = sf::VideoMode::getDesktopMode();
                Window = new sf::RenderWindow(video, title, sf::Style::None);
                Window->setPosition({0, 0});
                break;

            case VideoMode::Fullscreen:
                Window = new sf::RenderWindow(video, title, sf::State::Fullscreen);
                break;

            case VideoMode::Desktop:
                video = sf::VideoMode::getDesktopMode();
                Window = new sf::RenderWindow(video, title, sf::State::Windowed);
                break;

            default:
                Window = new sf::RenderWindow(video, title, sf::State::Windowed);
                break;
            }
        }

        namespace{
            static inline void BuildSprite(const ID id, sf::Sprite& sprite) {
                sprite.setPosition(transform::Positions[id]);
                sprite.setRotation(transform::Rotations[id]);
                sprite.setColor(sprite::Colors[id]);

                sf::Vector2f finalScale = transform::Scales[id];
                if(sprite::ShouldFlipXs[id]) finalScale.x = -finalScale.x;
                if(sprite::ShouldFlipYs[id]) finalScale.y = -finalScale.y;

                sprite.setScale(finalScale);
            }

            static inline void DrawParticles(){
                if(!Window) return;

                for(ID id = 1; id < MAX_ENTITIES; ++id){
                    if(particle::Positions[id].empty()) continue;

                    sf::CircleShape shape;
                    const size_t count = particle::Positions[id].size();

                    for(size_t i = 0; i < count; ++i){
                        shape.setRadius(particle::CurrentSizes[id][i]);
                        shape.setFillColor(particle::CurrentColors[id][i]);
                        shape.setPosition(particle::Positions[id][i]);
                        shape.setOrigin({
                            particle::CurrentSizes[id][i],
                            particle::CurrentSizes[id][i]}
                        );
                        Window->draw(shape);
                    }
                }
            }
        }

        static inline void Render(){
            if(!Window) return;

            for(ID id = 1; id < MAX_ENTITIES; ++id){
                if(!entity::IsAvailables[id] || !sprite::Textures[id]) continue;

                const std::vector<sf::IntRect>& frames
                    = animation::FrameSets[animation::States[id]];

                sf::Sprite sprite = frames.empty() ?
                    sf::Sprite(*sprite::Textures[id]) :
                    sf::Sprite(*sprite::Textures[id], frames[animation::CurrFrames[id]]);

                BuildSprite(id, sprite);
                Window->draw(sprite);
            }
            // DrawParticles();
        }
    }
}
