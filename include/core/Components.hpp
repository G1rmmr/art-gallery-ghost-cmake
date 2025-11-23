#pragma once

#include <SFML/Graphics.hpp>

#include <array>

namespace mir {
    static constexpr std::uint8_t MAX_ENTITIES = 0xFF;

    namespace transform {
        inline std::array<sf::Vector2f, MAX_ENTITIES> Positions;
        inline std::array<sf::Vector2f, MAX_ENTITIES> Velocities;
        inline std::array<float, MAX_ENTITIES> Rotations;
    }

    namespace physics {
        static constexpr float GRAV_ACCEL = 9.8f;

        inline std::array<sf::FloatRect, MAX_ENTITIES> Colliders;
        inline std::array<float, MAX_ENTITIES> Masses;
        inline std::array<bool, MAX_ENTITIES> InAirFlags;
    }

    namespace drawable {
        struct Animation {
            std::vector<sf::IntRect> Frames{};
            float Delay = 0.f;
            float Elapsed = 0.f;
            std::size_t CurrFrame = 0;
            bool IsLooping = false;
            bool IsPlaying = false;
        };

        inline std::array<Animation, MAX_ENTITIES> Animations;
        inline std::array<sf::Texture, MAX_ENTITIES> Textures;
        inline std::array<sf::Vector2f, MAX_ENTITIES> Scales;
    }
}
