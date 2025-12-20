#pragma once

#include <string>

#include <SFML/Graphics/Color.hpp>

#include "../core/Manager.hpp"
#include "../util/Types.hpp"
#include "../handle/Event.hpp"
#include "../core/Components.hpp"
#include "../core/Entity.hpp"

namespace mir{
    namespace ui{
        static inline void BuildText(
            const Tag tag,
            const sf::Color& color,
            const Point2<Real>& origin,
            const String& content,
            const Uint size){
            if(!font::Sources[tag]) return;

            font::Texts[tag]->setFillColor(color);
            font::Texts[tag]->setString(content);
            font::Texts[tag]->setCharacterSize(size);

            const Rect<Real> bounds = font::Texts[tag]->getLocalBounds();
            font::Texts[tag]->setOrigin({
                bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f   
            });

            font::Texts[tag]->setPosition(origin);
        }

        static inline ID BuildButton(
            const Action<const mir::event::type::MousePressed&>& onClick,
            const sf::Color& background,
            const Point2<Real>& pos,
            const Point2<Real>& size,
            const Tag tag = 0,
            const String& label = "",
            const sf::Color& foreground = {0, 0, 0, 0}){

            const ID id = entity::Create();
            if(id == 0) return 0;

            transform::Positions[id] = pos;
            transform::Scales[id] = {1, 1};
            physics::Bounds[id] = size;

            if(background.a != 0){
                sprite::Colors[id] = background;
                sprite::Types[id] = sprite::Type::Rectangle;
                texture::AllocFromType(id);
            }

            if(tag != 0 && label != "" && foreground.a != 0){
                const Point2<Real> center = {pos.x + size.x / 2, pos.y + size.y / 2};
                BuildText(tag, foreground, center, label, 30);
            }

            Action<const mir::event::type::MousePressed&> action
                = [id, pos, size, onClick](const event::type::MousePressed& e){
                const Real cursorX = TypeCast<Real>(e.X);
                const Real cursorY = TypeCast<Real>(e.Y);

                if(cursorX < pos.x) return;
                if(cursorX > pos.x + size.x) return;
                if(cursorY < pos.y) return;
                if(cursorY > pos.y + size.y) return;

                onClick(e);
            };
            event::Subscribe(action);

            return id;
        }
    }
}
