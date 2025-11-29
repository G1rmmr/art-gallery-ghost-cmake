#pragma once

#include "Mir.hpp"

class Game {
public:
    Game();
    void Run();

private:
    void initWindow();
    void update(const float deltaTime);
    void render();
};
