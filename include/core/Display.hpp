#pragma once

#include "Manager.hpp"

namespace mir{
    static inline sf::RenderWindow Window;

    enum class VideoMode{
        Windowed,
        Fullscreen,
        Borderless,
        Desktop
    };

    enum class Resolution{
        HD, // 1280x720
        FullHD, // 1920x1080
        QHD, // 2560x1440
        UHD, // 3840x2160
        Custom
    };

    namespace window{
        static inline bool IsOpening() { return Window.isOpen();}
        static inline void Display(){ Window.display(); }
        static inline void SetFPS(const std::uint8_t fps){ Window.setFramerateLimit(fps); }
        static inline void Close(){ Window.close(); }

        static inline void Init(
            const std::string& title,
            VideoMode mode = VideoMode::Desktop,
            Resolution res = Resolution::FullHD,
            uint32_t width = 0,
            uint32_t height = 0){
            sf::VideoMode video;

            switch(res){
            case Resolution::HD:
                video = sf::VideoMode({1280, 720});
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
                video = sf::VideoMode({1920, 1080});
                break;
            }

            sf::State state = sf::State::Windowed;
            switch(mode){
            case VideoMode::Fullscreen:
                state = sf::State::Fullscreen;
                break;

            case VideoMode::Borderless:
                state = sf::State::Windowed;
                // Borderless는 별도 처리 필요
                break;

            case VideoMode::Desktop:
                video = sf::VideoMode::getDesktopMode();
                break;

            case VideoMode::Windowed:
            default:
                state = sf::State::Windowed;
                break;
            }

            Window = sf::RenderWindow(video, title, state);
        }

        static inline void Clear(std::uint8_t red, std::uint8_t green, std::uint8_t blue){
            Window.clear(sf::Color(red, green, blue));
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
        }

        static inline void Render(){
            for(mir::ID id = 0; id < mir::MAX_ENTITIES; ++id){
                if(!mir::sprite::Textures[id]) continue;

                const std::vector<sf::IntRect>& frames
                    = mir::animation::FrameSets[mir::animation::States[id]];

                sf::Sprite sprite = frames.empty() ?
                    sf::Sprite(*mir::sprite::Textures[id]) :
                    sf::Sprite(*mir::sprite::Textures[id], frames[mir::animation::CurrFrames[id]]);

                BuildSprite(id, sprite);
                Window.draw(sprite);
            }
        }
    }
}
