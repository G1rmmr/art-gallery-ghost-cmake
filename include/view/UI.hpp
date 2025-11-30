#pragma once

#include "../core/Entity.hpp"

namespace mir{
    namespace ui{
        static inline ID CreateText(const std::string& content, float x, float y){
            return 0;
        }

        static inline void SetText(ID id, const std::string& content){
            return;
        }

        static inline void SetTextColor(ID id, std::uint8_t r, std::uint8_t g, std::uint8_t b){
            return;
        }

        static inline void SetTextSize(ID id, std::uint32_t size){
            return;
        }

        static inline ID CreateButton(const std::string& label,
            float x, float y, float width, float height){
            return 0;
        }

        static inline void OnButtonClick(ID id, std::function<void()> callback){
            return;
        }

        static inline ID CreateProgressBar(
            float x, float y, float width, float height){
            return 0;
        }

        static inline void SetProgress(ID id, float value){
            return;
        }

        static inline ID CreateImage(const char* texturePath, float x, float y){
            return 0;
        }
    }
}
