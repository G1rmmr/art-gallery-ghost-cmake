#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>

#include "../core/Entity.hpp"
#include "../util/Math.hpp"
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

        static inline sf::Vector2f GetCenter(){ return View.getCenter(); }
        static inline void Follow(ID target){ TargetID = target; }
        static inline void SetZoom(float zoom){ CurrentZoom = zoom; }

        static inline void SetPosition(const sf::Vector2f& pos){
            TargetID = 0;
            View.setCenter(pos);
        }

        static inline void SetPosition(float x, float y){
            TargetID = 0;
            View.setCenter(x, y);
        }

        static inline void Shake(float intensity, float duration){
            ShakePower = intensity;
            ShakeTime = duration;
        }

        static inline void Init(){
            if(Window){
                const sf::Vector2f size = static_cast<sf::Vector2f>(Window->getSize());
                View.setSize(size);
                View.setCenter(size / 2.f);
            }
        }

        static inline void Update(const float deltaTime){
            sf::Vector2f start = View.getCenter();
            sf::Vector2f end = start;

            if(TargetID != 0 && entity::IsAvailables[TargetID]){
                sf::Vector2f size = physics::Bounds[TargetID];
                sf::Vector2f target = transform::Positions[TargetID] + size / 2.f;

                float interval = deltaTime * 5.0f;
                if(interval > 1.0f) interval = 1.0f;

                end = math::Lerp(start, target, interval);
            }

            if(ShakeTime > 0){
                ShakeTime -= deltaTime;
                float offsetX = math::GetRandomReal(-50, 50) * ShakePower;
                float offsetY = math::GetRandomReal(-50, 50) * ShakePower;
                end += sf::Vector2f(offsetX, offsetY);
            }

            View.setCenter(end);

            if(Window) Window->setView(View);
        }
    }
}
