#pragma once

#include "core/Entity.hpp"
#include "core/Manager.hpp"

namespace mir{
    namespace ui{
        static inline void BuildText(
            const Tag tag,
            const sf::Vector2f& pos,
            const std::string& content,
            const std::uint8_t size){
            if(!font::Sources[tag]) return;

            font::Texts[tag]->setString(content);
            font::Texts[tag]->setPosition(pos);
            font::Texts[tag]->setCharacterSize(size);
        }

        static inline ID BuildButton(
            const std::string& label,
            float x, float y, float width, float height){
            return 0;
        }
    }
}
