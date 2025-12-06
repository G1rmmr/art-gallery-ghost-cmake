#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>

#include "../core/Entity.hpp"

namespace mir{
    namespace event{
        namespace type{
            struct Collision { ID Other; };
            struct Attack { ID Other; };
            struct Damaged { ID Other; };
            struct Death {};
            struct Jump {};
            struct ButtonClick {};

            enum class Key{
                Escape,
                Space,
                Enter,
                W,
                A,
                S,
                D
            };

            struct KeyPressed{
                Key Input;
            };

            struct KeyReleased{
                Key Input;
            };

            struct MousePressed{
                int X;
                int Y;
            };

            struct MouseReleased{
                int X;
                int Y;
            };
        }

        namespace{
            using SubID = std::size_t;

            class Base{
            public:
                virtual ~Base() = default;
            };

            template<typename T>
            class Listener : public Base{
            public:
                explicit Listener(std::function<void(const T&)> cb) : callback(std::move(cb)) {}
                void Exec(const T& event){ callback(event); }

            private:
                std::function<void(const T&)> callback;
            };

            static inline std::unordered_map<std::type_index,
                std::unordered_map<SubID, std::unique_ptr<Base>>> Listeners;

            static inline SubID NextID = 0;
        }

        template<typename T>
        static inline SubID Subscribe(std::function<void(const T&)> callback){
            std::type_index typeId = std::type_index(typeid(T));
            SubID id = NextID++;

            Listeners[typeId][id] = std::make_unique<Listener<T>>(std::move(callback));
            return id;
        }

        static inline void Unsubscribe(SubID id){
            for(auto& [typeId, listenerMap] : Listeners){
                if(listenerMap.erase(id) > 0) return;
            }
        }

        template<typename T>
        static inline void Publish(const T& event){
            std::type_index typeId = std::type_index(typeid(T));
            if(Listeners.find(typeId) == Listeners.end()) return;

            std::vector<Base*> currentListeners;
            currentListeners.reserve(Listeners.at(typeId).size());

            for(const auto& [_, pointer] : Listeners.at(typeId)){
                currentListeners.push_back(pointer.get());
            }

            for(Base* listener : currentListeners){
                if(listener) static_cast<Listener<T>*>(listener)->Exec(event);
            }
        }

        static inline void Clear(){ Listeners.clear(); }
    }
}
