#pragma once

#include "../core/Entity.hpp"

namespace mir{
    namespace ui{
        inline static ID CreateText(const std::string& content, float x, float y){

        }

        inline static void SetText(ID id, const std::string& content){

        }

        inline static void SetTextColor(ID id, std::uint8_t r, std::uint8_t g, std::uint8_t b){

        }

        inline static void SetTextSize(ID id, std::uint32_t size){

        }

        inline static ID CreateButton(const std::string& label,
            float x, float y, float width, float height){

        }

        inline static void OnButtonClick(ID id, std::function<void()> callback){

        }

        inline static ID CreateProgressBar(
            float x, float y, float width, float height){

        }

        inline static void SetProgress(ID id, float value){

        }

        inline static ID CreateImage(const char* texturePath, float x, float y){

        }
    }
}
