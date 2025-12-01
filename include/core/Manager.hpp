#pragma once

#include <cstddef>
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

#ifndef ASSET_DIR
    #define ASSET_DIR ""
#endif

namespace mir {
    static constexpr std::uint8_t MAX_RESOURCES = 0xFF;

    using Tag = std::uint8_t;

    namespace resource{
        static inline std::array<std::string, MAX_RESOURCES> Textures;
        static inline std::array<std::string, MAX_RESOURCES> Sounds;
        static inline std::array<std::string, MAX_RESOURCES> Fonts;
    }

    namespace texture{
        static inline Tag Create(const std::string& filepath){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag){
                if(resource::Textures[tag] == ""){
                    resource::Textures[tag] = std::string(ASSET_DIR) + '/' + filepath;
                    return tag;
                }
            }

            debug::Log("resource Overflow!");
            return 0; // Overflow resouce
        }

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

        static inline void Delete(const ID id){
            if(sprite::Textures[id])
                delete sprite::Textures[id];
        }

        static inline void DeleteAll(){
            for(ID id = 1; id < MAX_ENTITIES; ++id)
                Delete(id);
        }
    }

    namespace sound{
        static inline std::array<sf::SoundBuffer*, MAX_RESOURCES> Buffers;
        static inline std::array<sf::Sound*, MAX_RESOURCES> Sounds;
        static inline std::array<sf::Music, MAX_RESOURCES> Musics;

        static inline Tag Create(const std::string& filepath){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag){
                if(resource::Sounds[tag] == ""){
                    resource::Sounds[tag] = std::string(ASSET_DIR) + '/' + filepath;
                    return tag;
                }
            }

            debug::Log("resource Overflow!");
            return 0; // Overflow resouce
        }

        static inline void AllocSound(const Tag tag){
            Buffers[tag] = new sf::SoundBuffer();
            if(!Buffers[tag]->loadFromFile(resource::Sounds[tag])){
                debug::Log("Sound Source doesn't exist : %s", resource::Sounds[tag].c_str());
                return;
            }

            Sounds[tag] = new sf::Sound(*Buffers[tag]);
        }

        static inline void AllocMusic(const Tag tag, bool shouldLoop = true){
            if(!Musics[tag].openFromFile(resource::Sounds[tag])){
                debug::Log("Music Source doesn't exist : %s", resource::Sounds[tag].c_str());
                return;
            }

            Musics[tag].setLooping(shouldLoop);
            Musics[tag].play();
        }

        static inline void Play(const Tag tag, bool shouldLoop = false) {
            if(!Sounds[tag]) return;
            Sounds[tag]->setLooping(shouldLoop);
            Sounds[tag]->play();
        }

        static inline void Pause(const Tag tag) {
            if(Sounds[tag]) Sounds[tag]->pause();
        }

        static inline void Stop(const Tag tag) {
            if(Sounds[tag]) Sounds[tag]->stop();
        }

        static inline void Delete(const Tag tag){
            if(Buffers[tag]) delete Buffers[tag];
            if(Sounds[tag]) delete Sounds[tag];
        }

        static inline void DeleteAll(){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag)
                Delete(tag);
        }
    }

    namespace font{
        static inline std::array<sf::Font*, MAX_RESOURCES> Sources;
        static inline std::array<sf::Text*, MAX_RESOURCES> Texts;

        static inline Tag Create(const std::string& filepath){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag){
                if(resource::Fonts[tag] == ""){
                    resource::Fonts[tag] = std::string(ASSET_DIR) + '/' + filepath;
                    return tag;
                }
            }

            debug::Log("resource Overflow!");
            return 0; // Overflow resouce
        }

        static inline void Alloc(const Tag tag){
            Sources[tag] = new sf::Font(resource::Fonts[tag]);
            if(!Sources[tag]){
                debug::Log("Font doesn't exist : %s", resource::Fonts[tag].c_str());
                return;
            }
            Texts[tag] = new sf::Text(*Sources[tag]);
        }

        static inline void Delete(const Tag tag){
            if(Sources[tag]) delete Sources[tag];
            if(Texts[tag]) delete Texts[tag];
        }

        static inline void DeleteAll(){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag)
                Delete(tag);
        }
    }
}
