#pragma once

#include "Components.hpp"
#include "Manager.hpp"

#include <cassert>

namespace mir {
    namespace movement {
        inline void Update(const ID id, const float deltaTime) {
            sf::Vector2f& velocity = transform::Velocities[id];

            if(physics::InAirFlags[id]) velocity.y += physics::GRAV_ACCEL * deltaTime;

            transform::Positions[id] += velocity * deltaTime;
        }
    }

    namespace render {
        inline void UpdateAnimation(const ID id, const float deltaTime) {
            drawable::Animation& anim = drawable::Animations[id];
            if(!anim.IsPlaying || anim.Frames.empty()) return;

            if(anim.Delay <= 0.f) {
                anim.CurrFrame = (anim.CurrFrame + 1) % anim.Frames.size();
                return;
            }

            anim.Elapsed += deltaTime;

            while(anim.Elapsed >= anim.Delay) {
                anim.Elapsed -= anim.Delay;
                ++anim.CurrFrame;

                if(anim.CurrFrame >= anim.Frames.size()) {
                    if(anim.IsLooping) {
                        anim.CurrFrame = 0;
                    }
                    else {
                        anim.CurrFrame = anim.Frames.size() - 1;
                        anim.IsPlaying = false;
                        break;
                    }
                }
            }
        }

        inline void Update(const ID id, const float deltaTime) {
            UpdateAnimation(id, deltaTime);

            sf::Sprite sprite(drawable::Textures[id]);
            sf::Vector2f position = transform::Positions[id];

            sprite.setPosition(position);
            sprite.setScale(drawable::Scales[id]);

            const drawable::Animation& anim = drawable::Animations[id];
            if(!anim.Frames.empty()) sprite.setTextureRect(anim.Frames[anim.CurrFrame]);

            Manager::GetInstance().Window.draw(sprite);
        }
    }

    namespace collision {
        // TODO : implement collision detection
    }
}
