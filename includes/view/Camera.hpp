#pragma once

#include <SFML/Window.hpp>

#include "../core/Entity.hpp"
#include "Display.hpp"

namespace mir{
    namespace camera{
        namespace{
            inline sf::View View;

            inline float CurrentZoom = 1.f;
            inline float ShakeTime = 0.f;
            inline float ShakePower = 0.f;

            inline ID TargetID = 0;
        }

        static inline void Follow(ID target){ TargetID = target; }
        static inline void SetZoom(float zoom){ CurrentZoom = zoom; }

        static inline void SetPosition(float x, float y){
            TargetID = 0;
            View.setCenter(x, y);
        }

        static inline void Shake(float intensity, float duration){
            ShakePower = intensity;
            ShakeTime = duration;
        }

        static inline void Update(const float deltaTime){
            if(TargetID != 0 && entity::IsAvailables[TargetID]){
                sf::Vector2f targetPos = transform::Positions[TargetID];
                sf::Vector2f size = physics::Bounds[TargetID];

                // GameView.setCenter(lerp(current, target, dt * speed));
                View.setCenter(targetPos + size / 2.f);
            }

            if(ShakeTime > 0){
                ShakeTime -= deltaTime;
                float offsetX = (rand() % 100 - 50) / 50.0f * ShakePower;
                float offsetY = (rand() % 100 - 50) / 50.0f * ShakePower;
                View.move(offsetX, offsetY);
            }

            if(Window) Window->setView(View);
        }
    }
}
