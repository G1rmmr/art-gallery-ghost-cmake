#pragma once

#include <cstdint>
#include <array>

#include "Components.hpp"

namespace mir{
    using ID = std::uint16_t;

    namespace entity{
        static inline std::array<bool, MAX_ENTITIES> IsAvailables;

        static inline void ClearParticles(const ID id){
            particle::Positions[id].clear();
            particle::Velocities[id].clear();
            particle::CurrentColors[id].clear();
            particle::CurrentSizes[id].clear();
            particle::CurrentLifeTimes[id].clear();
            particle::MaxLifeTimes[id].clear();
            particle::EmitAccumulators[id] = 0.0f;
        }

        static inline void BurstParticle(const ID id, const std::uint16_t count){
            particle::EmitAccumulators[id] += static_cast<float>(count);
            particle::IsEmittings[id] = true;
        }
    }
}
