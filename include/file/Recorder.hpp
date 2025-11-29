#pragma once

#include <ios>
#include <vector>
#include <fstream>

#include "../core/Components.hpp"
#include "../util/Debugger.hpp"

#define REGISTER_COMPONENT(type, array) \
    { #type, reinterpret_cast<void*>(array.data()), sizeof(array[0]), array.size() }

namespace mir{
    namespace record{
        struct ComponentInfo{
            const char* Name;
            void* Data;
            size_t Size;
            size_t Count;
        };

        namespace{
            static inline const std::vector<ComponentInfo> GetAllComponents(){
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

        static inline void SaveAll(const std::string& filename){
            std::ofstream file(filename, std::ios::binary);

            const std::vector<ComponentInfo>& components = GetAllComponents();
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

        static inline void LoadAll(const std::string& filename){
            std::ifstream file(filename, std::ios::binary);

            size_t componentCount = 0;
            file.read(reinterpret_cast<char*>(&componentCount), sizeof(size_t));

            const std::vector<ComponentInfo>& components = GetAllComponents();
            for(const ComponentInfo& info : components){
                std::basic_string<char>::size_type nameLen = 0;
                file.read(reinterpret_cast<char*>(&nameLen), sizeof(size_t));

                std::string name(nameLen, '\0');
                file.read(name.data(), static_cast<std::streamsize>(nameLen));

                std::streamsize size = 0;
                std::streamsize count = 0;

                file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
                file.read(reinterpret_cast<char*>(&count), sizeof(size_t));

                std::__wrap_iter<const ComponentInfo*> iter = std::find_if(
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
}
