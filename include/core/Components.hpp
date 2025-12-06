#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "../util/Timer.hpp"

namespace mir{
    namespace transform{
        static inline std::array<sf::Vector2f, MAX_ENTITIES> Positions;
        static inline std::array<sf::Vector2f, MAX_ENTITIES> Velocities;
        static inline std::array<sf::Vector2f, MAX_ENTITIES> Scales;
        static inline std::array<float, MAX_ENTITIES> Rotations;

        static inline void Clear(){
            for(ID id = 1; id < MAX_ENTITIES; ++id){
                if(!entity::IsAvailables[id]) continue;

                Positions[id] = sf::Vector2f(0.f, 0.f);
                Velocities[id] = sf::Vector2f(0.f, 0.f);
                Scales[id] = sf::Vector2f(0.f, 0.f);
                Rotations[id] = 0.f;
            }
        }
    }

    namespace physics{
        static constexpr float GRAV_ACCEL = 980.f;

        static inline std::array<sf::Vector2f, MAX_ENTITIES> Bounds;
        static inline std::array<float, MAX_ENTITIES> Masses;

        static inline std::array<bool, MAX_ENTITIES> IsGhosts;
        static inline std::array<bool, MAX_ENTITIES> InAirFlags;

        static inline void Clear(){
            for(ID id = 1; id < MAX_ENTITIES; ++id){
                if(!entity::IsAvailables[id]) continue;

                Bounds[id] = sf::Vector2f(0.f, 0.f);
                Masses[id] = 0.f;
                IsGhosts[id] = false;
                InAirFlags[id] = false;
            }
        }
    }

    namespace sprite{
        enum class Type{
            None,
            Rectangle,
            Circle,
        };

        static inline std::array<sf::Color, MAX_ENTITIES> Colors;
        static inline std::array<sf::Vector2f, MAX_ENTITIES> Sizes;

        static inline std::array<std::unique_ptr<sf::Texture>, MAX_ENTITIES> Textures;

        static inline std::array<Type, MAX_ENTITIES> Types;
        static inline std::array<std::uint8_t, MAX_ENTITIES> Layers;

        static inline std::array<bool, MAX_ENTITIES> ShouldFlipXs;
        static inline std::array<bool, MAX_ENTITIES> ShouldFlipYs;

        static inline void Clear(){
            for(ID id = 1; id < MAX_ENTITIES; ++id){
                if(!entity::IsAvailables[id]) continue;

                Colors[id] = sf::Color(0, 0, 0);
                Sizes[id] = sf::Vector2f(0.f, 0.f);
                Textures[id].reset();
                Types[id] = Type::None;
                Layers[id] = 0;
                ShouldFlipXs[id] = false;
                ShouldFlipYs[id] = false;
            }
        }
    }

    namespace animation{
        static constexpr std::uint8_t MAX_STATE = 0x08;
        static constexpr std::uint8_t MAX_FRAME = 0xFF;

        using Set = std::vector<sf::IntRect>;

        static inline std::array<Set, MAX_ENTITIES> FrameSets;
        static inline std::array<float, MAX_ENTITIES> ElapsedTimes;
        static inline std::array<float, MAX_ENTITIES> DelayTimes;
        static inline std::array<std::uint8_t, MAX_STATE> States;
        static inline std::array<std::uint8_t, MAX_FRAME> CurrFrames;
        static inline std::array<bool, MAX_ENTITIES> IsPlayings;
        static inline std::array<bool, MAX_ENTITIES> IsLoopings;

        static inline void Clear(){
            for(ID id = 1; id < MAX_ENTITIES; ++id){
                if(!entity::IsAvailables[id]) continue;

                FrameSets[id] = Set();
                ElapsedTimes[id] = 0.f;
                DelayTimes[id] = 0.f;
                States[id] = 0;
                CurrFrames[id] = 0;
                IsPlayings[id] = false;
                IsLoopings[id] = false;
            }
        }
    }

    namespace particle{
        static inline std::array<std::vector<sf::Vector2f>, MAX_ENTITIES> Positions;
        static inline std::array<std::vector<sf::Vector2f>, MAX_ENTITIES> Velocities;
        static inline std::array<std::vector<sf::Color>, MAX_ENTITIES> CurrentColors;
        static inline std::array<std::vector<float>, MAX_ENTITIES> CurrentSizes;
        static inline std::array<std::vector<float>, MAX_ENTITIES> CurrentLifeTimes;
        static inline std::array<std::vector<float>, MAX_ENTITIES> MaxLifeTimes;
        static inline std::array<sf::Color, MAX_ENTITIES> StartColors;
        static inline std::array<sf::Color, MAX_ENTITIES> EndColors;
        static inline std::array<float, MAX_ENTITIES> EmitRates;
        static inline std::array<float, MAX_ENTITIES> EmitAccumulators;
        static inline std::array<float, MAX_ENTITIES> StartSizes;
        static inline std::array<float, MAX_ENTITIES> EndSizes;
        static inline std::array<float, MAX_ENTITIES> TargetLifeTimes;
        static inline std::array<std::uint16_t, MAX_ENTITIES> MaxParticles;
        static inline std::array<bool, MAX_ENTITIES> IsEmittings;

        static inline void Delete(const ID id){
            Positions[id].clear();
            Velocities[id].clear();
            CurrentColors[id].clear();
            CurrentSizes[id].clear();
            CurrentLifeTimes[id].clear();
            MaxLifeTimes[id].clear();
            EmitAccumulators[id] = 0.0f;
        }

        static inline void BurstParticle(const ID id, const std::uint16_t count){
            EmitAccumulators[id] += static_cast<float>(count);
            IsEmittings[id] = true;

            time::After(0.01f, [id](){ particle::IsEmittings[id] = false; });
        }
    }

    namespace stats{
        static constexpr std::uint16_t MAX_HEALTH = 0xFFFF;
        static constexpr std::uint8_t MAX_DAMAGE = 0xFF;

        static inline std::array<float, MAX_ENTITIES> Healths;
        static inline std::array<float, MAX_ENTITIES> Damages;

        static inline void Clear(){
            for(ID id = 1; id < MAX_ENTITIES; ++id){
                if(!entity::IsAvailables[id]) continue;
                Healths[id] = 0.f;
                Damages[id] = 0.f;
            }
        }
    }
}
