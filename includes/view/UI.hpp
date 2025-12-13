#pragma once

#include <string>
#include <cstdint>

#include <SFML/Graphics.hpp>

#include "../core/Manager.hpp"
#include "../util/Types.hpp"

namespace mir{
    namespace ui{
        static inline void BuildText(
            const Tag tag,
            const sf::Color& color,
            const Point2<Real>& pos,
            const String& content,
            const Uint size){
            if(!font::Sources[tag]) return;

            font::Texts[tag]->setFillColor(color);
            font::Texts[tag]->setString(content);
            font::Texts[tag]->setPosition(pos);
            font::Texts[tag]->setCharacterSize(size);
        }

        static inline ID BuildButton(
            const String& label,
            Real x, Real y, Real width, Real height){
            return 0;
        }
    }
}
