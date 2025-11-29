#pragma once

#include <SFML/Graphics.hpp>

#include <cassert>
#include <cstdint>
#include <optional>
#include <vector>

#include "Components.hpp"
#include "Event.hpp"
#include "core/Manager.hpp"
#include "util/Debugger.hpp"

namespace mir {
    namespace movement {
        static inline void Update(const float deltaTime) {
            for(ID id = 0; id < MAX_ENTITIES; ++id){
                if(!entity::IsAvailables[id]) return;

                sf::Vector2f& velocity = transform::Velocities[id];
                if(physics::InAirFlags[id]) velocity.y += physics::GRAV_ACCEL * deltaTime;
                transform::Positions[id] += velocity * deltaTime;
            }
        }
    }

    namespace render {
        inline void Update(const float deltaTime) {
            for(ID id = 0; id < MAX_ENTITIES; ++id){
                if(!entity::IsAvailables[id]) return;

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
    }

    namespace collision{
        namespace{
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
        }

        static inline void Update(){
            std::vector<ID> activated;

            for(ID id = 0; id < MAX_ENTITIES; ++id){
                if(mir::entity::IsAvailables[id])
                    activated.push_back(id);
            }

            const ID lastID = static_cast<ID>(activated.size());

            for(ID id = 0; id < lastID - 1; ++id){
                for(ID other = id + 1; other < MAX_ENTITIES; ++other){
                    if(IsCollide(physics::RectColliders[id], physics::RectColliders[other])){
                        event::type::Coliision event{id, other};
                        event::Publish(event);
                    }
                }
            }
        }
    }
}
