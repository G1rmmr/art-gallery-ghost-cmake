#pragma once

#include <cstdint>
#include <limits>

#include <functional>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <queue>
#include <stack>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "core/Components.hpp"
#include "core/Entity.hpp"
#include "core/Manager.hpp"
#include "core/Systems.hpp"

#include "file/Recorder.hpp"

#include "handle/Event.hpp"
#include "handle/Input.hpp"

#include "util/Debugger.hpp"
#include "util/Profiler.hpp"
#include "util/Timer.hpp"
#include "util/Math.hpp"

#include "view/Camera.hpp"
#include "view/Display.hpp"
#include "view/Scene.hpp"
#include "view/UI.hpp"

namespace mir {
    template<typename T>
    using List = std::vector<T>;

    template<typename K, typename V>
    using Dictionary = std::unordered_map<K, V>;

    template<typename T>
    using Queue = std::queue<T>;

    template<typename T>
    using Stack = std::stack<T>;

    template<typename... TArgs>
    using Action = std::function<void(TArgs...)>;

    template<typename TReturn, typename... TArgs>
    using Func = std::function<TReturn(TArgs...)>;

    template<typename T>
    using Point2 = sf::Vector2<T>;

    template<typename T>
    using Rect = sf::Rect<T>;

    using Int = std::int32_t;
    using Uint = std::uint16_t;
    using Real = float;
    using Bool = bool;
    using String = std::string;
    using Color = sf::Color;
    using Texture = sf::Texture;
    using Sprite = sf::Sprite;
    using Sound = sf::Sound;
    using Music = sf::Music;
    using Font = sf::Font;
    using Text = sf::Text;

    static constexpr Int I_MAX = std::numeric_limits<Int>::max();
    static constexpr Int I_MIN = std::numeric_limits<Int>::min();
    static constexpr Uint U_MAX = std::numeric_limits<Uint>::max();
    static constexpr Uint U_MIN = std::numeric_limits<Uint>::min();
    static constexpr Real R_MAX = std::numeric_limits<Real>::max();
    static constexpr Real R_MIN = std::numeric_limits<Real>::min();
}
