#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>

#include "Manager.hpp"

namespace mir{
    namespace event{
        static constexpr std::uint8_t MAX_EVENTS = 0xFF;

        namespace type{
            struct Coliision {
                ID EntityA;
                ID EntityB;
            };

            struct Damage {
                ID Attacker;
                ID Target;
            };

            struct Death {
                ID EntityId;
            };

            struct Jump {
                ID EntityId;
            };
        }

        template<typename T>
        using Listener = std::function<void(const T&)>;

        template<typename T>
        inline static std::array<std::optional<Listener<T>>, MAX_EVENTS> Listeners;

        template<typename T>
        inline static void Subscribe(const Tag tag, const Listener<T> listener) {
            Listeners<T>[tag] = listener;
        }

        template<typename T>
        inline static void Unsubscribe(const Tag tag) { Listeners<T>[tag].reset(); }

        template<typename T>
        inline static void Publish(const T& event){
            for(const std::optional<Listener<T>> listener : Listeners<T>){
                if(listener.has_value()) (*listener)(event);
            }
        }

        template<typename T>
        inline static void Clear(){
            for(std::optional<Listener<T>> listener : Listeners<T>)
                listener.reset();
        }
    }
}
