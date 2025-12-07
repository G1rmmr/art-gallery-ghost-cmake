#pragma once

#include <chrono>
#include <cstdio>
#include <ctime>
#include <utility>

#include "Timer.hpp"

#ifdef DEBUG_MODE
    static constexpr bool DEBUG_ENABLED = true;
#else
    static constexpr bool DEBUG_ENABLED = false;
#endif

namespace mir {
    namespace debug {
        struct DebugState {
            bool IsFpsVisible = true;
            bool IsEntityCountVisible = true;
            bool IsColliderVisible = true;
            float CurrentFPS = 0.f;
        };

        inline DebugState State;

        template<typename... Args>
        static inline void Log(const char* format, Args&&... args){
            if constexpr(!DEBUG_ENABLED) return;

            time::Set nowLocalTimeSet = time::GetLocalTime();
            std::fprintf(stdout, "%02d:%02d:%02d.%03d\t",
                nowLocalTimeSet.Hour,
                nowLocalTimeSet.Minute,
                nowLocalTimeSet.Second,
                nowLocalTimeSet.MiliSec);

            if constexpr(sizeof...(Args) == 0) std::printf("%s", format);
            else std::printf(format, std::forward<Args>(args)...);

            std::printf("\n");
            std::fflush(stdout);
        }

        static inline void ShowEntityCount(bool isEnabled){ State.IsEntityCountVisible = isEnabled; }
        static inline void ShowColliders(bool isEnabled){ State.IsColliderVisible = isEnabled; }

        static inline void ToggleFPS() { State.IsFpsVisible = !State.IsFpsVisible; }
        static inline void ToggleOverlay() {
            bool newState = !State.IsColliderVisible;
            State.IsColliderVisible = newState;
            State.IsEntityCountVisible = newState;
            State.IsFpsVisible = newState;
        }
    }
}
