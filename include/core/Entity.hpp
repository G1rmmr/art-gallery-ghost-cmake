#pragma once

#include "../util/Debugger.hpp"

namespace mir{
    using ID = std::uint16_t;

    static constexpr ID MAX_ENTITIES = 0xFFFF;

    namespace entity{
        static inline std::array<bool, MAX_ENTITIES> IsAvailables;

        static inline ID Create(){
            for(ID id = 1; id < MAX_ENTITIES; ++id){
                if(!IsAvailables[id]){
                    IsAvailables[id] = true;
                    return id;
                }
            }

            debug::Log("Entity Overflow!");
            return 0; // Overflow entity
        }

        static inline void ClearAll(){
            for(ID id = 1; id < MAX_ENTITIES; ++id){
                IsAvailables[id] = false;
            }
        }
    }
}
