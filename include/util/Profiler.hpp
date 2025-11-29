#pragma once

#include <chrono>
#include <string>

#include "Debugger.hpp"

#ifndef ENABLE_PROFILE
    #define ENABLE_PROFILE 0
#endif

namespace mir{
    namespace profile{
        class ScopeTimer{
        public:
            ScopeTimer(std::string_view name) : boundName(name){
                start = std::chrono::high_resolution_clock::now();
            }

            ~ScopeTimer(){
                std::chrono::high_resolution_clock::time_point end
                    = std::chrono::high_resolution_clock::now();

                std::chrono::microseconds duration
                    = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                debug::Log("[PROFILE] %s: %lld μs", boundName, duration.count());
            }
        private:
            std::chrono::high_resolution_clock::time_point start;
            std::string_view boundName;
        };
    }
}

#ifdef ENABLE_PROFILE
    #define PROFILE_SCOPE(name) if(mir::profile::ScopeTimer _timer##__LINE__(name); true)
#else
    #define PPROFILE_SCOPE(name) if(true)
#endif
