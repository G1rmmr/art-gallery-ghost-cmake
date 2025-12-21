#pragma once

#include <Mir>

#include "Network.hpp"

namespace player{
    const mir::String NAME = "JIWON";

    const mir::Real SPEED = 200;
    const mir::Real MAX_HEALTH = 100;
    const mir::Real INIT_DAMAGE = 10;

    const mir::Uint SIZE = 50;

    const mir::Bool IS_GHOST = false;
    const mir::Bool IN_AIR = true;

    namespace{
        inline void InitTransform(const mir::ID id){
            mir::transform::Positions[id] = mir::Point2<mir::Real>(
                mir::Window->getView().getCenter().x / 2.0, mir::Window->getView().getCenter().y / 2.0
            );
            mir::transform::Scales[id] = mir::Point2<mir::Real>(1, 1);
        }

        inline void InitPhysics(const mir::ID id){
            mir::physics::Bounds[id] = mir::Point2<mir::Real>(SIZE, SIZE);
            mir::physics::IsGhosts[id] = IS_GHOST;
            mir::physics::InAirFlags[id] = IN_AIR;
        }

        inline void InitSprite(const mir::ID id){
            mir::sprite::Colors[id] = mir::Color::Red;
            mir::sprite::Types[id] = mir::sprite::Type::Circle;
            mir::sprite::Layers[id] = 1;
            mir::texture::AllocFromType(id);
        }

        inline void InitStats(const mir::ID id){
            mir::stats::Healths[id] = MAX_HEALTH;
            mir::stats::Damages[id] = INIT_DAMAGE;
        }

        inline void SubscribeKeyPressed(const mir::ID id){
            mir::Action<const mir::event::type::KeyPressed&> action
                = [id](const mir::event::type::KeyPressed& event){
                switch(event.Input){
                case mir::event::type::Key::W:
                    mir::transform::Velocities[id].y = -SPEED;
                    break;

                case mir::event::type::Key::A:
                    mir::transform::Velocities[id].x = -SPEED;
                    break;

                case mir::event::type::Key::S:
                    mir::transform::Velocities[id].y = SPEED;
                    break;

                case mir::event::type::Key::D:
                    mir::transform::Velocities[id].x = SPEED;
                    break;

                case mir::event::type::Key::Space:
                    mir::camera::Follow(id);
                    break;

                default: break;
                }
            };
            mir::event::Subscribe(action);
        }

        inline void SubscribeKeyReleased(const mir::ID id){
            mir::Action<const mir::event::type::KeyReleased&> action
                = [id](const mir::event::type::KeyReleased& event){
                switch(event.Input){
                case mir::event::type::Key::A:
                case mir::event::type::Key::D:
                    mir::transform::Velocities[id].x = 0;
                    break;

                case mir::event::type::Key::W:
                case mir::event::type::Key::S:
                    mir::transform::Velocities[id].y = 0; 
                    break;

                default: break;
                }
            };
            mir::event::Subscribe(action);
        }
    }

    inline mir::ID Create(){
        const mir::ID id = mir::entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitPhysics(id);
        InitSprite(id);
        InitStats(id);

        SubscribeKeyPressed(id);
        SubscribeKeyReleased(id);

        return id;
    }
}
