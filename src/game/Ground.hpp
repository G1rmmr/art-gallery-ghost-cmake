#pragma once

#include <Mir>

namespace ground{
    const mir::Real POS_X = -100;
    const mir::Real POS_Y = -100;

    const mir::Int SIZE_X = 2000;
    const mir::Int SIZE_Y = 2000;

    namespace{
        inline void InitTransform(const mir::ID id){
            mir::transform::Positions[id] = mir::Point2<mir::Real>(POS_X, POS_Y);
            mir::transform::Scales[id] = mir::Point2<mir::Real>(1, 1);
        }

        inline void InitPhysics(const mir::ID id){
            mir::physics::Bounds[id] = mir::Point2<mir::Real>(SIZE_X, SIZE_Y);
            mir::physics::IsGhosts[id] = true;
        }

        inline void InitStats(const mir::ID id) {
            mir::stats::Healths[id] = 999999;
        }

        inline void GenerateRandomTerrain() {
            mir::Int randomCount = mir::math::GetRandomInt(8, 12);
            for(mir::Size i = 0; i < randomCount; ++i) {
                const mir::ID id = mir::entity::Create();

                mir::transform::Positions[id] = mir::math::GetRandomPoint({POS_X, POS_Y}, {SIZE_X, SIZE_Y});

                mir::transform::Scales[id] = mir::Point2<mir::Real>(1, 1);

                const mir::Real size = mir::math::GetRandomReal(100.0, 300.0);
                mir::physics::Bounds[id] = {size, size};
                mir::physics::IsGhosts[id] = true;

                mir::sprite::Colors[id] = mir::Color::Black;
                mir::sprite::Types[id] = mir::sprite::Type::ConvexHull;
                mir::sprite::Layers[id] = 1;

                mir::sprite::NumSide[id] = mir::math::GetRandomInt(5, 9);
                mir::texture::AllocFromType(id);

                InitStats(id);
            }
        }

        inline void InitSprite(const mir::ID id){
            mir::sprite::Colors[id] = mir::Color(127, 127, 127);
            mir::sprite::Types[id] = mir::sprite::Type::Rectangle;
            mir::sprite::Layers[id] = 0;
            mir::texture::AllocFromType(id);

            GenerateRandomTerrain();
        }
    }

    inline mir::ID Create(){
        const mir::ID id = mir::entity::Create();
        if(id == 0) return 0;

        InitTransform(id);
        InitPhysics(id);
        InitSprite(id);
        InitStats(id);

        return id;
    }
}
