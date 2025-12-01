#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

#include "core/Entity.hpp"

namespace mir{
    namespace event{
        namespace type{
            struct Collision{ ID Other; };
            struct Attack{ ID Other; };
            struct Damaged{ ID Other; };
            struct Death{};
            struct Jump{};
            struct ButtonClick{};
        }

        template<typename T>
        using Listener = std::function<void(const T&)>;

        template<typename T>
        static inline std::vector<Listener<T>> Listeners;

        template<typename T>
        static inline void Subscribe(const Listener<T>& listener) {
            Listeners<T>.push_back(listener);
        }

        template<typename T>
        static inline void Publish(const T& event){
            for(Listener<T>& listener : Listeners<T>)
                listener(event);
        }

        static inline void ClearAll(){
            Listeners<type::Collision>.clear();
            Listeners<type::Attack>.clear();
            Listeners<type::Damaged>.clear();
            Listeners<type::Death>.clear();
            Listeners<type::Jump>.clear();
            Listeners<type::ButtonClick>.clear();
        }
    }
}
