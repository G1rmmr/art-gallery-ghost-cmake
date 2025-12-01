#pragma once

#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <type_traits>

#ifdef DEBUG_MODE
    static inline constexpr bool DEBUG_ENABLED = true;
#else
    static inline constexpr bool DEBUG_ENABLED = false;
#endif

namespace mir{
    namespace debug{
        template<typename... Args>
        static inline void Log(const char* format, Args&&... args){
            if constexpr(!DEBUG_ENABLED) return;

            const std::chrono::time_point now = std::chrono::system_clock::now();
            const time_t time = std::chrono::system_clock::to_time_t(now);
            const std::chrono::milliseconds ms = duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) % 1000;

            std::tm* localTime = std::localtime(&time);
            fprintf(stdout, "%02d:%02d:%02d.%03d\t",
                localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
                static_cast<int>(ms.count()));

            if constexpr(sizeof...(Args) == 0) printf("%s", format);
            else printf(format, std::forward<Args>(args)...);

            printf("\n");
            fflush(stdout);
        }

        static inline void ShowFPS(bool enabled){
            return;
        }

        static inline void ShowEntityCount(bool enabled){
            return;
        }

        static inline void ShowColliders(bool enabled){
            return;
        }
    }
}
