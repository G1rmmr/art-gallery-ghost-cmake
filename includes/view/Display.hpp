#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>

#include "../core/Components.hpp"
#include "../core/Entity.hpp"
#include "../core/Manager.hpp"
#include "../util/Debugger.hpp"
#include "../util/Profiler.hpp"
#include "../util/Types.hpp"

namespace mir{
    
    inline HandledWindow Window = nullptr;

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
        static inline Bool IsOpening() { return Window && Window->isOpen(); }

        static inline void Display(){
            if(Window) Window->display();
        }

        static inline Point2<Uint> GetDisplayResolution() {
            const Point2<Uint> resolution = {
                TypeCast<Uint>(DisplayMode::getDesktopMode().size.x),
                TypeCast<Uint>(DisplayMode::getDesktopMode().size.y)
            };
            return resolution;
        }

        static inline void SetFPS(const Uint fps){
            if(Window) {
                Window->setFramerateLimit(fps);
                Window->setVerticalSyncEnabled(true);
            }
        }

        static inline void Close(){
            if(Window) Window->close();
        }

        static inline void Clear(Uint red, Uint green, Uint blue){
            if(Window) Window->clear(Color(red, green, blue));
        }

        static inline void Clear(const Color& color) {
            if(Window) Window->clear(color);
        }

        static inline void Shutdown(){
            if(Window){
                Window->close();
                delete Window;
                Window = nullptr;
            }
        }

        static inline void Init(
            const String& title,
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

            switch(mode){
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
                Window = new sf::RenderWindow(video, title, sf::Style::Default);
                break;

            default:
                Window = new sf::RenderWindow(video, title, sf::Style::Default);
                break;
            }
        }

        namespace{
            static inline void BuildSprite(const ID id, sf::Sprite& sprite){
                sprite.setPosition(transform::Positions[id]);
                sprite.setRotation(sf::degrees(transform::Rotations[id]));
                sprite.setColor(sprite::Colors[id]);

                Point2<Real> finalScale = transform::Scales[id];
                if(sprite::ShouldFlipXs[id]) finalScale.x = -finalScale.x;
                if(sprite::ShouldFlipYs[id]) finalScale.y = -finalScale.y;

                sprite.setScale(finalScale);
            }

            static inline void DrawSprites(){
                static List<ID> renderQueue;
                renderQueue.clear();

                for(ID id = 1; id < MAX_ENTITIES; ++id){
                    if(sprite::Textures[id] && entity::IsAvailables[id])
                        renderQueue.push_back(id);
                }

                std::sort(renderQueue.begin(), renderQueue.end(), [](ID a, ID b){
                    if(sprite::Layers[a] != sprite::Layers[b])
                        return sprite::Layers[a] < sprite::Layers[b];
                    return transform::Positions[a].y < transform::Positions[b].y;
                });

                for(const ID id : renderQueue){
                    const List<sf::Rect<Int>>& frames
                        = animation::FrameSets[animation::States[id]];

                    sf::Sprite sprite = frames.empty() ?
                        sf::Sprite(*sprite::Textures[id]) :
                        sf::Sprite(*sprite::Textures[id], frames[animation::CurrFrames[id]]);

                    BuildSprite(id, sprite);
                    Window->draw(sprite);
                }
            }

            static inline void DrawParticles(){
                for(ID id = 1; id < MAX_ENTITIES; ++id){
                    if(particle::Positions[id].empty()) continue;

                    const Size count = particle::Positions[id].size();
                    PointArr vertexArr(PrimType::Triangles, count * 6);

                    for (size_t i = 0; i < count; ++i) {
                        const Point2<Real>& pos = particle::Positions[id][i];
                        const Real size = particle::CurrentSizes[id][i];
                        const Color& color = particle::CurrentColors[id][i];

                        const Real halfSize = size * 0.5f;

                        Point2<Real> tl(pos.x - halfSize, pos.y - halfSize);
                        Point2<Real> tr(pos.x + halfSize, pos.y - halfSize);
                        Point2<Real> br(pos.x + halfSize, pos.y + halfSize);
                        Point2<Real> bl(pos.x - halfSize, pos.y + halfSize);

                        vertexArr[i * 6 + 0].position = tl; vertexArr[i * 6 + 0].color = color;
                        vertexArr[i * 6 + 1].position = tr; vertexArr[i * 6 + 1].color = color;
                        vertexArr[i * 6 + 2].position = br; vertexArr[i * 6 + 2].color = color;

                        vertexArr[i * 6 + 3].position = br; vertexArr[i * 6 + 3].color = color;
                        vertexArr[i * 6 + 4].position = bl; vertexArr[i * 6 + 4].color = color;
                        vertexArr[i * 6 + 5].position = tl; vertexArr[i * 6 + 5].color = color;
                    }
                    Window->draw(vertexArr);
                }
            }

            static inline void DrawTexts(){
                for(Tag tag = 1; tag < MAX_RESOURCES; ++tag){
                    if(font::Sources[tag] && font::Texts[tag])
                        Window->draw(*font::Texts[tag]);
                }
            }

            static inline void DrawDebug(){
                if(!Window) return;

                static sf::Font font;
                static Bool fontLoaded = false;
                static std::unique_ptr<sf::Text> text;

                if(!fontLoaded){
                    if(font.openFromFile("assets/fonts/dieproud.ttf")){
                        fontLoaded = true;
                        text = std::make_unique<sf::Text>(font);
                        text->setCharacterSize(24);
                        text->setFillColor(sf::Color::Green);
                    }
                }

                if constexpr(DEBUG_ENABLED) {
                    if(mir::debug::IsEntityCountVisible && fontLoaded && text){
                        sf::View oldView = Window->getView();
                        Window->setView(Window->getDefaultView());

                        mir::ID count = 0;
                        for(Bool available : entity::IsAvailables)
                            if(available) count++;

                        text->setString("[Toggle - F1] Entities: " + std::to_string(count));
                        text->setPosition({10.f, 10.f});

                        Window->draw(*text);
                        Window->setView(oldView);
                    }

                    if(mir::debug::IsColliderVisible){
                        for(ID id = 1; id < MAX_ENTITIES; ++id){
                            if(!entity::IsAvailables[id]) continue;

                            const Point2<Real>& size = physics::Bounds[id];
                            if(size.x == 0 && size.y == 0) continue;

                            sf::RectangleShape rect(size);
                            rect.setPosition(transform::Positions[id]);
                            rect.setFillColor(sf::Color::Transparent);
                            rect.setOutlineColor(sf::Color::Red);
                            rect.setOutlineThickness(1);

                            Window->draw(rect);
                        }
                    }
                }
            }

            static inline void DrawProfile(){
                if(!Window) return;

                static sf::Font font;
                static Bool fontLoaded = false;
                static std::unique_ptr<sf::Text> text;

                if(!fontLoaded){
                    if(font.openFromFile("assets/fonts/dieproud.ttf")){
                        fontLoaded = true;
                        text = std::make_unique<sf::Text>(font);
                        text->setCharacterSize(24);
                        text->setFillColor(sf::Color::Green);
                    }
                }

                if(mir::profile::IsEnable && fontLoaded && text){
                    sf::View oldView = Window->getView();
                    Window->setView(Window->getDefaultView());

                    const Real fps = mir::profile::CurrentFPS;
                    text->setString("[Toggle - F2] FPS: " + std::to_string(TypeCast<Int>(fps)));
                    text->setPosition({10.f, 40.f});

                    Window->draw(*text);
                    Window->setView(oldView);
                }
            }
        }

        static inline void Render(){
            if(!Window) return;

            DrawSprites();
            DrawParticles();
            DrawTexts();
            DrawDebug();
            DrawProfile();
        }
    }
}
