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

#include "handle/Event.hpp"
#include "handle/Input.hpp"

#include "util/Debugger.hpp"
#include "util/Profiler.hpp"
#include "util/Timer.hpp"
#include "util/Math.hpp"
#include "util/Types.hpp"

#include "view/Camera.hpp"
#include "view/Display.hpp"
#include "view/Scene.hpp"
#include "view/UI.hpp"

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/html5.h>
#endif
