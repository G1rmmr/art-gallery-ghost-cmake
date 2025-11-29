#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <format>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Components.hpp"
#include "util/Debugger.hpp"

namespace mir {
    const std::uint8_t MAX_RESOURCES = 0xFF;

    using Tag = std::uint8_t;

    namespace resource{
        static inline std::array<std::string, MAX_RESOURCES> Textures;
        static inline std::array<std::unique_ptr<sf::Sound>, MAX_RESOURCES> Sources;
    }

    namespace texture{
        static inline void Alloc(const ID id, const Tag tag){
            const std::string path = resource::Textures[tag];
            if(path == "") {
                debug::Log("Texture doesn't initialized : %s", path.c_str());
                return;
            }

            std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
            if (!texture->loadFromFile(path)) {
                debug::Log("Texture doesn't exist : %s", path.c_str());
                return;
            }
            sprite::Textures[id] = std::move(texture);
        }
    }

    namespace sound{
        static inline std::array<bool, MAX_RESOURCES> ShouldLoops;

        static inline void Alloc(const Tag tag, const std::string& filepath){
            std::unique_ptr<sf::SoundBuffer> buffer = std::make_unique<sf::SoundBuffer>();
            if(buffer->loadFromFile(filepath)){
                debug::Log("Sound Source doesn't exist : %s", filepath.c_str());
                return;
            }

            std::unique_ptr<sf::Sound> source = std::make_unique<sf::Sound>(*buffer);

            resource::Sources[tag] = std::move(source);
            resource::Sources[tag]->setLooping(ShouldLoops[tag]);
        }
    }
}
