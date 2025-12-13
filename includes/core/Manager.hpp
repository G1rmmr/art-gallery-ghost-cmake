#pragma once

#include <string>
#include <memory>
#include <vector>
#include <fstream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Components.hpp"
#include "../handle/Event.hpp"
#include "../util/Debugger.hpp"
#include "../util/Types.hpp"

#ifndef ASSET_DIR
    #define ASSET_DIR ""
#endif

#define REGISTER_COMPONENT(type, array) \
    { #type, reinterpret_cast<void*>(array.data()), sizeof(array[0]), array.size() }

namespace mir {
    static constexpr Uint MAX_RESOURCES = 0xFF;

    using Tag = Uint;

    namespace record{
        namespace{
            struct ComponentInfo{
                const char* Name;
                void* Data;
                std::size_t Size;
                std::size_t Count;
            };

            static inline const List<ComponentInfo> GetAllComponents(){
                return {
                    REGISTER_COMPONENT("Position", transform::Positions),
                    REGISTER_COMPONENT("Velocity", transform::Velocities),
                    REGISTER_COMPONENT("Scale", transform::Scales),
                    REGISTER_COMPONENT("Rotation", transform::Rotations),
                    REGISTER_COMPONENT("Mass", physics::Masses),
                    REGISTER_COMPONENT("Health", stats::Healths),
                };
            }
        }

        static inline void Save(const String& filename){
            std::ofstream file(filename, std::ios::binary);

            const List<ComponentInfo>& components = GetAllComponents();
            size_t componentCount = components.size();

            file.write(reinterpret_cast<const char*>(&componentCount), sizeof(size_t));

            for(const ComponentInfo& comp : components){
                std::streamsize nameLen = static_cast<std::streamsize>(std::strlen(comp.Name));
                file.write(reinterpret_cast<const char*>(&nameLen), sizeof(size_t));
                file.write(comp.Name, nameLen);

                file.write(reinterpret_cast<const char*>(&comp.Size), sizeof(size_t));
                file.write(reinterpret_cast<const char*>(&comp.Count), sizeof(size_t));

                std::streamsize dataSize = static_cast<std::streamsize>(comp.Size * comp.Count);
                file.write(reinterpret_cast<const char*>(comp.Data), dataSize);
            }

            file.close();
            debug::Log("Saved %zu components", componentCount);
        }

        static inline void Load(const String& filename){
            std::ifstream file(filename, std::ios::binary);

            if(!file.is_open()){
                debug::Log("No save file found: %s", filename.c_str());
                return;
            }

            size_t componentCount = 0;
            file.read(reinterpret_cast<char*>(&componentCount), sizeof(size_t));

            const List<ComponentInfo>& components = GetAllComponents();
            for(size_t i = 0; i < componentCount; ++i){
                std::basic_string<char>::size_type nameLen = 0;
                file.read(reinterpret_cast<char*>(&nameLen), sizeof(size_t));

                String name(nameLen, '\0');
                file.read(name.data(), static_cast<std::streamsize>(nameLen));

                std::streamsize size = 0;
                std::streamsize count = 0;

                file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
                file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

                List<ComponentInfo>::const_iterator iter = std::find_if(
                    components.begin(), components.end(),[&name](const ComponentInfo& c){
                        return name == c.Name;
                    });

                if(iter == components.end()){
                    file.seekg(size * count, std::ios::cur);
                    debug::Log("Unknown component: %s", name.c_str());
                    continue;
                }
                file.read(reinterpret_cast<char*>(iter->Data), size * count);
            }

            file.close();
            debug::Log("Loaded %zu components", componentCount);
        }
    }

    namespace resource{
        static inline Array<String, MAX_RESOURCES> Textures;
        static inline Array<String, MAX_RESOURCES> Sounds;
        static inline Array<String, MAX_RESOURCES> Fonts;
    }

    namespace texture{
        static inline Tag Create(const String& filepath){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag){
                if(resource::Textures[tag] == ""){
                    resource::Textures[tag] = String(ASSET_DIR) + '/' + filepath;
                    return tag;
                }
            }

            debug::Log("resource Overflow!");
            return 0; // Overflow resource
        }

        static inline void AllocFromType(const ID id){
            Point2<Real> size = physics::Bounds[id];
            if(size.x == 0 && size.y == 0){
                debug::Log("This entity doesn't have size (ID : %d)", id);
                return;
            }

            RenderTex shape;
            if (!shape.create(static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y))) {
                debug::Log("Failed to create render texture for entity (ID: %d)", id);
                return;
            }
            shape.clear(Color::Transparent);

            switch(sprite::Types[id]){
            case sprite::Type::Rectangle: {
                Rectangle rect(static_cast<Point2<Real>>(size));
                rect.setFillColor(sprite::Colors[id]);
                shape.draw(rect);
                break;
            }
            case sprite::Type::Circle:{
                Circle circle(static_cast<Real>(size.x) / 2);
                circle.setFillColor(sprite::Colors[id]);
                shape.draw(circle);
                break;
            }
            default:
                debug::Log("This entity doesn't have shape (ID : %d)", id);
                return;
            }

            shape.display();

            sprite::Textures[id] = std::make_unique<Texture>(shape.getTexture());
        }

        static inline void AllocFromFile(const ID id, const Tag tag){
            const String path = resource::Textures[tag];
            if(path == "") {
                debug::Log("Texture doesn't initialized : %s", path.c_str());
                return;
            }

            std::unique_ptr<Texture> texture = std::make_unique<Texture>();
            if (!texture->loadFromFile(path)) {
                debug::Log("Texture doesn't exist : %s", path.c_str());
                return;
            }
            sprite::Textures[id] = std::move(texture);
        }

        static inline void Delete(const ID id){
            sprite::Textures[id].reset();
        }

        static inline void Clear(){
            for(ID id = 1; id < MAX_ENTITIES; ++id)
                Delete(id);
        }
    }

    namespace sound{
        static inline Array<std::unique_ptr<SoundBuf>, MAX_RESOURCES> Buffers;
        static inline Array<std::unique_ptr<Sound>, MAX_RESOURCES> Sounds;
        static inline Array<Music, MAX_RESOURCES> Musics;

        static inline Tag Create(const String& filepath){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag){
                if(resource::Sounds[tag] == ""){
                    resource::Sounds[tag] = String(ASSET_DIR) + '/' + filepath;
                    return tag;
                }
            }

            debug::Log("resource Overflow!");
            return 0; // Overflow resource
        }

        static inline void AllocSound(const Tag tag){
            Buffers[tag] = std::make_unique<SoundBuf>();
            if(!Buffers[tag]->loadFromFile(resource::Sounds[tag])){
                debug::Log("Sound Source doesn't exist : %s", resource::Sounds[tag].c_str());
                return;
            }

            Sounds[tag] = std::make_unique<Sound>(*Buffers[tag]);
        }

        static inline void AllocMusic(const Tag tag, Bool shouldLoop = true){
            if(!Musics[tag].openFromFile(resource::Sounds[tag])){
                debug::Log("Music Source doesn't exist : %s", resource::Sounds[tag].c_str());
                return;
            }

            Musics[tag].setLoop(shouldLoop);
            Musics[tag].play();
        }

        static inline void Play(const Tag tag, Bool shouldLoop = false) {
            if(!Sounds[tag]) return;
            Sounds[tag]->setLoop(shouldLoop);
            Sounds[tag]->play();
        }

        static inline void Pause(const Tag tag) {
            if(Sounds[tag]) Sounds[tag]->pause();
        }

        static inline void Stop(const Tag tag) {
            if(Sounds[tag]) Sounds[tag]->stop();
            if(Musics[tag].getStatus() != SoundSrc::Stopped) Musics[tag].stop();
        }

        static inline void Delete(const Tag tag){
            Stop(tag);
            Buffers[tag].reset();
            Sounds[tag].reset();
        }

        static inline void Clear(){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag)
                Delete(tag);
        }
    }

    namespace font{
        static inline Array<std::unique_ptr<Font>, MAX_RESOURCES> Sources;
        static inline Array<std::unique_ptr<Text>, MAX_RESOURCES> Texts;

        static inline Tag Create(const String& filepath){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag){
                if(resource::Fonts[tag] == ""){
                    resource::Fonts[tag] = String(ASSET_DIR) + '/' + filepath;
                    return tag;
                }
            }

            debug::Log("resource Overflow!");
            return 0; // Overflow resouce
        }

        static inline void Alloc(const Tag tag){
            Sources[tag] = std::make_unique<Font>();
            if(!Sources[tag]->loadFromFile(resource::Fonts[tag])){
                debug::Log("Font doesn't exist : %s", resource::Fonts[tag].c_str());
                Sources[tag].reset();
                return;
            }
            Texts[tag] = std::make_unique<Text>();
            Texts[tag]->setFont(*Sources[tag]);
        }

        static inline void Delete(const Tag tag){
            Sources[tag].reset();
            Texts[tag].reset();
        }

        static inline void Clear(){
            for(Tag tag = 1; tag < MAX_RESOURCES; ++tag)
                Delete(tag);
        }
    }

    static inline void Clear(){
        transform::Clear();
        sprite::Clear();
        animation::Clear();
        stats::Clear();
        entity::Clear();
        event::Clear();

        texture::Clear();
        sound::Clear();
        font::Clear();
    }
}
