#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <format>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Components.hpp"
#include "util/Debugger.hpp"

namespace mir {
    static constexpr std::uint8_t MAX_RESOURCES = 0xFF;

    using Tag = std::uint8_t;

    namespace resource{
        static inline std::array<std::string, MAX_RESOURCES> Textures;
        static inline std::array<std::unique_ptr<sf::Sound>, MAX_RESOURCES> Sources;
    }

    namespace texture{
        static inline void AllocFromType(const ID id){
            sf::Vector2f size = physics::Bounds[id];
            if(size.x == 0 && size.y == 0){
                debug::Log("This entity doesn't have size (ID : %d)", id);
                return;
            }

            sf::RenderTexture shape(static_cast<sf::Vector2u>(physics::Bounds[id]));
            shape.clear(sf::Color::Transparent);

            switch(sprite::Types[id]){
            case sprite::Type::Rectangle: {
                sf::RectangleShape rect(static_cast<sf::Vector2f>(size));
                rect.setFillColor(sprite::Colors[id]);
                shape.draw(rect);
                break;
            }
            case sprite::Type::Circle:{
                sf::CircleShape circle(static_cast<float>(size.x) / 2.f);
                circle.setFillColor(sprite::Colors[id]);
                shape.draw(circle);
                break;
            }
            default:
                debug::Log("This entity doesn't have shape (ID : %d)", id);
                return;
            }

            shape.display();

            sprite::Textures[id] = new sf::Texture(shape.getTexture());
        }

        static inline void AllocFromFile(const ID id, const Tag tag){
            const std::string path = resource::Textures[tag];
            if(path == "") {
                debug::Log("Texture doesn't initialized : %s", path.c_str());
                return;
            }

            sf::Texture* texture = new sf::Texture();
            if (!texture->loadFromFile(path)) {
                debug::Log("Texture doesn't exist : %s", path.c_str());
                return;
            }
            sprite::Textures[id] = texture;
        }

        static inline void DeleteAll(){
            for(ID id = 0; id < MAX_ENTITIES; ++id){
                if(sprite::Textures[id])
                    delete sprite::Textures[id];
            }
        }
    }

    namespace sound{
        static inline std::array<bool, MAX_RESOURCES> ShouldLoops;

        static inline void Alloc(const Tag tag, const std::string& filepath){
            std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique<sf::SoundBuffer>();
            if(!buffer->loadFromFile(filepath)){
                debug::Log("Sound Source doesn't exist : %s", filepath.c_str());
                return;
            }

            std::unique_ptr<sf::Sound> source = std::make_unique<sf::Sound>(*buffer);

            resource::Sources[tag] = std::move(source);
            resource::Sources[tag]->setLooping(ShouldLoops[tag]);
        }
    }
}
