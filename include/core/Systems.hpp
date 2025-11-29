#pragma once

#include <SFML/Graphics.hpp>

#include <cassert>
#include <cstdint>
#include <optional>
#include <vector>

#include "Entity.hpp"
#include "Components.hpp"

#include "../handle/Event.hpp"
#include "../util/Debugger.hpp"

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

    namespace effect{
        namespace{
            static inline void GenerateParticles(const ID id, const float deltaTime){
                if(!particle::IsEmittings[id]) return;

                particle::EmitAccumulators[id] += particle::EmitRates[id] * deltaTime;

                const sf::Vector2f& emitterPos = transform::Positions[id];
                const std::size_t particleCount = particle::Positions[id].size();

                while(particle::EmitAccumulators[id] >= 1.0f){
                    particle::EmitAccumulators[id] -= 1.0f;

                    if(particleCount < particle::MaxParticles[id]){
                        float angle = (rand() % 360) * 3.14159f / 180.0f;
                        float speed = 50.0f + (rand() % 100);

                        particle::Positions[id].push_back(emitterPos);
                        particle::Velocities[id].push_back({
                            std::cos(angle) * speed,
                            std::sin(angle) * speed
                        });
                        particle::CurrentColors[id].push_back(particle::StartColors[id]);
                        particle::CurrentSizes[id].push_back(particle::StartSizes[id]);
                        particle::CurrentLifeTimes[id].push_back(particle::TargetLifeTimes[id]);
                        particle::MaxLifeTimes[id].push_back(particle::TargetLifeTimes[id]);
                    }
                }
            }

            static inline void KillParticles(const ID id, const std::uint16_t index){
                particle::Positions[id].erase(particle::Positions[id].begin() + index);
                particle::Velocities[id].erase(particle::Velocities[id].begin() + index);
                particle::CurrentColors[id].erase(particle::CurrentColors[id].begin() + index);
                particle::CurrentSizes[id].erase(particle::CurrentSizes[id].begin() + index);
                particle::CurrentLifeTimes[id].erase(particle::CurrentLifeTimes[id].begin() + index);
                particle::MaxLifeTimes[id].erase(particle::MaxLifeTimes[id].begin() + index);
            }

            static inline void UpdateParticles(const ID id, const float deltaTime){
                const std::size_t particleCount = particle::Positions[id].size();

                for(std::size_t i = particleCount - 1; i >= 0; --i){
                    particle::CurrentLifeTimes[id][i] -= deltaTime;

                    if(particle::CurrentLifeTimes[id][i] <= 0.0f){
                        KillParticles(id, i);
                        continue;
                    }

                    particle::Positions[id][i] += particle::Velocities[id][i] * deltaTime;
                    particle::Velocities[id][i].y += physics::GRAV_ACCEL * deltaTime;

                    float t = 1.0f - (particle::CurrentLifeTimes[id][i] / particle::MaxLifeTimes[id][i]);

                    const sf::Color& start = particle::StartColors[id];
                    const sf::Color& end = particle::EndColors[id];

                    particle::CurrentColors[id][i] = sf::Color(
                        start.r + (end.r - start.r) * static_cast<std::uint8_t>(t),
                        start.g + (end.g - start.g) * static_cast<std::uint8_t>(t),
                        start.b + (end.b - start.b) * static_cast<std::uint8_t>(t),
                        start.a + (end.a - start.a) * static_cast<std::uint8_t>(t)
                    );

                    particle::CurrentSizes[id][i] = particle::StartSizes[id]
                        + (particle::EndSizes[id] - particle::StartSizes[id]) * t;
                }
            }
        }

        static inline void Update(const float deltaTime){
            for(ID id = 0; id < MAX_ENTITIES; ++id){
                if(!particle::IsEmittings[id] && particle::Positions[id].empty()) continue;

                GenerateParticles(id, deltaTime);
                UpdateParticles(id, deltaTime);
            }
        }
    }
}
