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

        struct Set{
            int Hour;
            int Minute;
            int Second;
            int MiliSec;
        };

        static inline Set GetLocalTime(){
            const std::chrono::time_point now = std::chrono::system_clock::now();
            const time_t time = std::chrono::system_clock::to_time_t(now);
            const std::chrono::milliseconds ms = duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) % 1000;

            std::tm* localTime = std::localtime(&time);

            return {
                localTime->tm_hour,
                localTime->tm_min,
                localTime->tm_sec,
                static_cast<int>(ms.count())
            };
        }

        static inline float GetDelta(){
            static sf::Clock deltaClock;
            return deltaClock.restart().asSeconds();
        }

        static inline void Register(
            const float seconds,
            std::function<void()> callback,
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
