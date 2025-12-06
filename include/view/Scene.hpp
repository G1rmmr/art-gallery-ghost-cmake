#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "../util/Debugger.hpp"
#include "../core/Manager.hpp"

namespace mir{
    namespace scene{
        inline std::unordered_map<std::string, std::function<void()>> Scenes;
        inline std::string Current = "";

        inline void Register(const std::string& name, std::function<void()> func){
            Scenes[name] = std::move(func);
        }

        inline void Load(const std::string& name) {
            if(Current == name) return;

            std::unordered_map<std::string, std::function<void()>>::iterator iter = Scenes.find(name);
            if(iter == Scenes.end()) {
                debug::Log("Attempted to load an unregistered scene: %s", name.c_str());
                return;
            }

            Current = name;
            mir::Clear();
            iter->second();
        }
    }
}
