#pragma once

#include <random>

#include <SFML/Graphics.hpp>

namespace mir{
    namespace math{
        static inline float Lerp(
            const float start, const float end, const float interval){
            return (1.f - interval) * start + interval * end;
        }

        static inline sf::Vector2f Lerp(
            const sf::Vector2f& start, const sf::Vector2f& end, const float interval){
            return {Lerp(start.x, end.x, interval), Lerp(start.y, end.y, interval)};
        }

        static inline sf::Color Lerp(
            const sf::Color& start, const sf::Color& end, const float interval){
            return {
                static_cast<std::uint8_t>(Lerp(start.r, end.r, interval)),
                static_cast<std::uint8_t>(Lerp(start.g, end.g, interval)),
                static_cast<std::uint8_t>(Lerp(start.b, end.b, interval))
            };
        }

        static inline std::int32_t GetRandomInt(const std::int32_t start, const std::int32_t end){
            std::random_device random;
            std::mt19937 gen(random());

            std::uniform_int_distribution<std::int32_t> dist(start, end);
            return dist(random);
        }

        static inline float GetRandomReal(const float start, const float end){
            std::random_device random;
            std::mt19937 gen(random());

            std::uniform_real_distribution<float> dist(start, end);
            return dist(random);
        }

        static inline sf::Vector2<float> GetRandomPoint(
            const sf::Vector2<float>& start, const sf::Vector2<float>& end){
            return {GetRandomReal(start.x, end.x), GetRandomReal(start.y, end.y)};
        }

        static inline sf::Color GetRandomColor(
            const sf::Color& start, const sf::Color& end){
            return {
                static_cast<std::uint8_t>(GetRandomInt(start.r, end.r)),
                static_cast<std::uint8_t>(GetRandomInt(start.g, end.g)),
                static_cast<std::uint8_t>(GetRandomInt(start.b, end.b))
            };
        }
    }
}
