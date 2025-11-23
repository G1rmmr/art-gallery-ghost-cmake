#pragma once

#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <type_traits>

#ifdef DEBUG_MODE
inline constexpr bool DEBUG_ENABLED = true;
#else
inline constexpr bool DEBUG_ENABLED = false;
#endif

namespace mir{
    class Debugger{
    public:
        template<typename... Args>
        static void Log(Args&&... args){
            if constexpr(DEBUG_ENABLED){
                std::ostringstream oss;
                oss << getTimeStamp() << "\t";
                (oss << ... << args);
                std::cout << oss.str() << "\n";
                std::cout.flush();
            }
        }

    private:
        static std::string getTimeStamp(){
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            time_t time = std::chrono::system_clock::to_time_t(now);
            std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) % 1000;

            std::ostringstream oss;

            oss << std::put_time(std::localtime(&time), "%H:%M:%S")
                << "." << std::setfill('0') << std::setw(3) << ms.count();

            return oss.str();
        }
    };
}
