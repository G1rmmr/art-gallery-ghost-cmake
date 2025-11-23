#pragma once

class Game {
public:
    Game();
    void Run();

private:
    void initWindow();
    void update(const float deltaTime);
};
