#pragma once

#include <SFML/Graphics.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cassert>
#include <cstdint>
#include <optional>
#include <vector>

#include "Components.hpp"
#include "Event.hpp"
#include "util/Debugger.hpp"

namespace mir {
    namespace movement {
        static inline void Update(const ID id, const float deltaTime) {
            sf::Vector2f& velocity = transform::Velocities[id];
            if(physics::InAirFlags[id]) velocity.y += physics::GRAV_ACCEL * deltaTime;
            transform::Positions[id] += velocity * deltaTime;
        }
    }

    namespace render {
        inline void Update(const ID id, const float deltaTime) {
            const std::vector<sf::IntRect>& frames = animation::FrameSets[animation::States[id]];
            if(!animation::IsPlayings[id] || frames.empty()) return;

            const std::uint8_t maxFrame = static_cast<std::uint8_t>(frames.size());
            if(animation::DelayTimes[id] <= 0.f) {
                animation::CurrFrames[id] = (animation::CurrFrames[id] + 1) % maxFrame;
                return;
            }

            animation::ElapsedTimes[id] += deltaTime;

            while(animation::ElapsedTimes[id] >= animation::DelayTimes[id]) {
                animation::ElapsedTimes[id] -= animation::DelayTimes[id];

                if(++animation::CurrFrames[id] >= maxFrame) {
                    if(!animation::IsLoopings[id]) {
                        animation::CurrFrames[id] = maxFrame - 1;
                        animation::IsPlayings[id] = false;
                        break;
                    }
                    animation::CurrFrames[id] = 0;
                }
            }
        }
    }

    namespace collision {
        static inline bool IsCollide(const sf::FloatRect& lhs, const sf::FloatRect& rhs){
            if(std::optional<sf::Rect<float>> bound = lhs.findIntersection(rhs)){
                debug::Log("Collision area in {%d} <-> {%d} is,", lhs, rhs);
                debug::Log("{%f, %f, %f, %f}",
                    bound->position.x, bound->position.y,
                    bound->position.x + bound->size.x,
                    bound->position.y + bound->size.y);
                return true;
            }

            debug::Log("No intersection in {%d} <-> {%d}", lhs, rhs);
            return false;
        }

        static inline void Update(const ID lhs, const ID rhs){
            if(IsCollide(physics::RectColliders[lhs], physics::RectColliders[rhs])){
                event::type::CollisionEvent event{lhs, rhs};
                event::Publish(event);
            }
        }
    }
}
