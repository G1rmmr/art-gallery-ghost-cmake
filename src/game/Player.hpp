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
        inline void InitTransform(const mir::ID id){
            mir::transform::Positions[id] = sf::Vector2f(POS_X, POS_Y);
            mir::transform::Scales[id] = sf::Vector2f(1.f, 1.f);
        }

        inline void InitPhysics(const mir::ID id){
            mir::physics::Bounds[id] = sf::Vector2f(SIZE_X, SIZE_Y);
            mir::physics::Masses[id] = MASS;
            mir::physics::InAirFlags[id] = IN_AIR;
            mir::physics::IsGhosts[id] = IS_GHOST;
        }

        inline void InitSprite(const mir::ID id){
            mir::sprite::Colors[id] = sf::Color(0xFF, 0xFF, 0xFF);
            mir::sprite::Types[id] = mir::sprite::Type::Rectangle;
            mir::texture::AllocFromType(id);
        }

        inline void InitStats(const mir::ID id){
            mir::stats::Healths[id] = MAX_HEALTH;
            mir::stats::Damages[id] = INIT_DAMAGE;
        }

        inline void SubscribeCollision(const mir::ID id){
            mir::event::Subscribe<mir::event::type::Collision>(
                [id](const mir::event::type::Collision& args){
                    float left = mir::transform::Positions[id].x;
                    float top = mir::transform::Positions[id].y;

                    float otherLeft = mir::transform::Positions[args.Other].x;
                    float otherTop = mir::transform::Positions[args.Other].y;
                    float otherRight = otherLeft + mir::physics::Bounds[args.Other].x;
                    float otherBottom = otherTop + mir::physics::Bounds[args.Other].y;

                    float dx = (otherLeft + mir::physics::Bounds[args.Other].x / 2.f)
                        - (left + mir::physics::Bounds[id].x / 2.f);

                    float dy = (otherTop + mir::physics::Bounds[args.Other].y / 2.f)
                        - (top + mir::physics::Bounds[id].y / 2.f);

                    float overlapX = (mir::physics::Bounds[id].x / 2.f
                        + mir::physics::Bounds[args.Other].x / 2.f)
                        - std::abs(dx);

                    float overlapY = (mir::physics::Bounds[id].y / 2.f
                        + mir::physics::Bounds[args.Other].y / 2.f)
                        - std::abs(dy);

                    if(overlapX < overlapY){
                        mir::transform::Positions[id].x = dx > 0 ?
                            otherLeft - mir::physics::Bounds[id].x :
                            otherRight;
                    }
                    else{
                        mir::transform::Positions[id].y = dy > 0 ?
                            otherTop - mir::physics::Bounds[id].y :
                            otherBottom;

                        if(dy > 0) mir::physics::InAirFlags[id] = false;
                    }
                }
            );
        }

        inline void SubscribeJump(const mir::ID id){
            mir::event::Subscribe<mir::event::type::Jump>(
                [id](const mir::event::type::Jump&){
                    if(mir::physics::InAirFlags[id]) return;

                    const float force = JUMP_POWER / std::sqrt(mir::physics::Masses[id]);

                    mir::transform::Velocities[id].y = -force;
                    mir::physics::InAirFlags[id] = true;
                }
            );
        }

        inline void SubscribeKeyPressed(const mir::ID id){
            mir::input::OnKeyPressed([id](const mir::input::Key key){
                switch(key){
                case mir::input::Key::W:
                    mir::event::Publish(mir::event::type::Jump{});
                    break;

                case mir::input::Key::A:
                    mir::transform::Velocities[id].x = -SPEED;
                    break;

                case mir::input::Key::D:
                    mir::transform::Velocities[id].x = SPEED;
                    break;

                default:
                    break;
                }
            });
        }

        inline void SubscribeKeyReleased(const mir::ID id){
            mir::input::OnKeyReleased([id](const mir::input::Key key){
                switch(key){
                case mir::input::Key::A:
                case mir::input::Key::D:
                    mir::transform::Velocities[id].x = 0.f;
                    break;

                default:
                    break;
                }
            });
        }
    }

    inline mir::ID Create(){
        const mir::ID id = mir::entity::Create();
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
