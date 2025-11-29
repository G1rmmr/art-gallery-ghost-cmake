#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System.hpp>

#include <array>
#include <memory>
#include <vector>

namespace mir{
    static constexpr std::uint8_t MAX_ENTITIES = 0xFF;

    namespace transform{
        static inline std::array<sf::Vector2f, MAX_ENTITIES> Positions;
        static inline std::array<sf::Vector2f, MAX_ENTITIES> Velocities;
        static inline std::array<sf::Vector2f, MAX_ENTITIES> Scales;
        static inline std::array<sf::Angle, MAX_ENTITIES> Rotations;
    }

    namespace physics{
        static constexpr float GRAV_ACCEL = 9.8f;

        static inline std::array<sf::FloatRect, MAX_ENTITIES> RectColliders;
        static inline std::array<float, MAX_ENTITIES> Masses;
        static inline std::array<bool, MAX_ENTITIES> InAirFlags;
    }

    namespace sprite{
        static inline std::array<sf::Color, MAX_ENTITIES> Colors;
        static inline std::array<std::unique_ptr<sf::Texture>, MAX_ENTITIES> Textures;
        static inline std::array<std::uint8_t, MAX_ENTITIES> Layers;
        static inline std::array<bool, MAX_ENTITIES> ShouldFlipXs;
        static inline std::array<bool, MAX_ENTITIES> ShouldFlipYs;
    }

    namespace animation{
        static constexpr std::uint8_t MAX_STATE = 0x08;
        static constexpr std::uint8_t MAX_FRAME = 0xFF;

        using Set = std::vector<sf::IntRect>;

        static inline std::array<Set, MAX_ENTITIES> FrameSets;
        static inline std::array<float, MAX_ENTITIES> ElapsedTimes;
        static inline std::array<float, MAX_ENTITIES> DelayTimes;
        static inline std::array<std::uint8_t, MAX_STATE> States;
        static inline std::array<std::uint8_t, MAX_FRAME> CurrFrames;
        static inline std::array<bool, MAX_ENTITIES> IsPlayings;
        static inline std::array<bool, MAX_ENTITIES> IsLoopings;
    }

    namespace stats{
        static constexpr std::uint16_t MAX_HEALTH = 0xFFFF;
        static constexpr std::uint8_t MAX_DAMAGE = 0xFF;

        static inline std::array<float, MAX_ENTITIES> Healths;
        static inline std::array<float, MAX_ENTITIES> Damages;
    }
}
