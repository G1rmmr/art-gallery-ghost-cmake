#pragma once

#include "Mir.hpp"

namespace player{
    const float POS_X = 200.f;
    const float POS_Y = 200.f;
    const float SPEED = 200.f;
    const float JUMP_POWER = 3000.f;
    const float MASS = 10.f;
    const float MAX_HEALTH = 100.f;
    const float INIT_DAMAGE = 10.f;

    const int SIZE_X = 100;
    const int SIZE_Y = 100;

    const bool IN_AIR = true;
    const bool IS_GHOST = false;

    namespace{
        inline void InitTransform(const ID id){
            transform::Positions[id] = {POS_X, POS_Y};
            transform::Scales[id] = {1.f, 1.f};
        }

        inline void InitPhysics(const ID id){
            physics::Bounds[id] = {SIZE_X, SIZE_Y};
            physics::Masses[id] = MASS;
            physics::InAirFlags[id] = IN_AIR;
            physics::IsGhosts[id] = IS_GHOST;
        }

        inline void InitSprite(const ID id){
            sprite::Colors[id] = {0xFF, 0xFF, 0xFF};
            sprite::Types[id] = sprite::Type::Rectangle;
            texture::AllocFromType(id);
        }

        inline void InitStats(const ID id){
            stats::Healths[id] = MAX_HEALTH;
            stats::Damages[id] = INIT_DAMAGE;
        }

        inline void SubscribeCollision(const ID id){
            event::Subscribe<event::type::Collision>([id](const event::type::Collision& args){
                float left = transform::Positions[id].x;
                float top = transform::Positions[id].y;

                float otherLeft = transform::Positions[args.Other].x;
                float otherTop = transform::Positions[args.Other].y;
                float otherRight = otherLeft + physics::Bounds[args.Other].x;
                float otherBottom = otherTop + physics::Bounds[args.Other].y;

                float dx = (otherLeft + physics::Bounds[args.Other].x / 2.f)
                    - (left + physics::Bounds[id].x / 2.f);

                float dy = (otherTop + physics::Bounds[args.Other].y / 2.f)
                    - (top + physics::Bounds[id].y / 2.f);

                float overlapX = (physics::Bounds[id].x / 2.f + physics::Bounds[args.Other].x / 2.f)
                    - std::abs(dx);

                float overlapY = (physics::Bounds[id].y / 2.f + physics::Bounds[args.Other].y / 2.f)
                    - std::abs(dy);

                if(overlapX < overlapY){
                    transform::Positions[id].x = dx > 0 ? otherLeft - physics::Bounds[id].x : otherRight;
                }
                else{
                    transform::Positions[id].y = dy > 0 ? otherTop - physics::Bounds[id].y : otherBottom;
                    if(dy > 0) physics::InAirFlags[id] = false;
                }
            });
        }

        inline void SubscribeJump(const ID id){
            event::Subscribe<event::type::Jump>([id](const event::type::Jump&){
                if(physics::InAirFlags[id]) return;

                const float force = JUMP_POWER / std::sqrt(physics::Masses[id]);

                transform::Velocities[id].y = -force;
                physics::InAirFlags[id] = true;
            });
        }

        inline void SubscribeKeyPressed(const ID id){
            input::OnKeyPressed([id](const input::Key key){
                switch(key){
                case input::Key::W:
                    event::Publish(event::type::Jump{});
                    break;

                case input::Key::A:
                    transform::Velocities[id].x = -SPEED;
                    break;

                case input::Key::D:
                    transform::Velocities[id].x = SPEED;
                    break;

                default:
                    break;
                }
            });
        }

        inline void SubscribeKeyReleased(const ID id){
            input::OnKeyReleased([id](const input::Key key){
                switch(key){
                case input::Key::A:
                case input::Key::D:
                    transform::Velocities[id].x = 0.f;
                    break;

                default:
                    break;
                }
            });
        }
    }

    inline ID Create(){
        const ID id = entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitPhysics(id);
        InitSprite(id);
        InitStats(id);

        SubscribeCollision(id);
        SubscribeJump(id);

        SubscribeKeyPressed(id);
        SubscribeKeyReleased(id);

        return id;
    }
}
