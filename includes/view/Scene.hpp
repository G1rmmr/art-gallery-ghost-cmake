#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "../util/Debugger.hpp"
#include "../core/Manager.hpp"
#include "../util/Types.hpp"

namespace mir{
    namespace scene{
        inline Dictionary<String, Action<>> Scenes;
        inline String Current = "";

        inline void Register(const String& name, std::function<void()> func){
            Scenes[name] = std::move(func);
        }

        inline void Load(const String& name) {
            if(Current == name) return;

            Dictionary<String, Action<>>::iterator iter = Scenes.find(name);
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
