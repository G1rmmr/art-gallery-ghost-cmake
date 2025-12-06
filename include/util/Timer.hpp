#pragma once

#include <vector>
#include <functional>

#include <SFML/System.hpp>

namespace mir{
    namespace time{
        namespace{
            struct TimerTask{
                std::function<void()> CallBack;
                float IntervalSec;
                float RemainSec;
                bool IsLooping;
            };

            inline std::vector<TimerTask> TimerTasks;
        }

        static inline float GetDelta(){
            static sf::Clock deltaClock;
            return deltaClock.restart().asSeconds();
        }

        static inline void Register(
            std::function<void()> callback,
            const float seconds,
            bool isLooping = false){
            TimerTasks.push_back({std::move(callback), seconds, seconds, isLooping});
        }

        static inline void Update(const float deltaTime){
            if(TimerTasks.empty()) return;

            for(TimerTask& task : TimerTasks){
                task.RemainSec -= deltaTime;

                if(task.RemainSec > 0.f) continue;
                if(task.CallBack) task.CallBack();
                if(task.IsLooping){
                    task.RemainSec += task.IntervalSec;
                    continue;
                }
            }

            std::erase_if(TimerTasks, [](const TimerTask& task) {
                return !task.IsLooping && task.RemainSec <= 0.f;
            });
        }
    }
}
