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
            mir::Int randomCount = mir::math::GetRandomInt(20, 25);

            mir::List<mir::Rect<mir::Int>> existingRects;
            for(mir::Size i = 0; i < randomCount; ++i){
                mir::Point2<mir::Real> pos;
                mir::Point2<mir::Real> size;
                mir::Rect<mir::Int> newRect;
                bool overlapped = false;
                int retries = 0;

                do{
                    pos = mir::math::GetRandomPoint({POS_X, POS_Y}, {SIZE_X, SIZE_Y});
                    mir::Real preSize = mir::math::GetRandomReal(100.0, 300.0);
                    size = {preSize, preSize};

                    newRect = mir::Rect<mir::Int>(
                        {mir::TypeCast<mir::Int>(pos.x), mir::TypeCast<mir::Int>(pos.y)},
                        {mir::TypeCast<mir::Int>(size.x), mir::TypeCast<mir::Int>(size.y)}
                    );

                    for(const mir::Rect<mir::Int>& rect : existingRects){
                        if (newRect.findIntersection(rect).has_value()){
                            overlapped = true;
                            break;
                        }
                    }
                    retries++;
                }while(overlapped && retries < 10);

                if(!overlapped){
                    const mir::ID id = mir::entity::Create();
                    mir::transform::Positions[id] = pos;
                    mir::transform::Scales[id] = mir::Point2<mir::Real>(1, 1);
                    mir::physics::Bounds[id] = size;
                    mir::physics::IsGhosts[id] = false;

                    mir::sprite::Colors[id] = mir::Color::Black;
                    mir::sprite::Types[id] = mir::sprite::Type::ConvexHull;
                    mir::sprite::Layers[id] = 1;
                    mir::sprite::NumSide[id] = mir::math::GetRandomInt(5, 9);
                    
                    mir::texture::AllocFromType(id);
                    InitStats(id);

                    existingRects.push_back(newRect);
                }
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
