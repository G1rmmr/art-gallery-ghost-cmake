#include "Game.hpp"

#include "core/Components.hpp"
#include "core/Systems.hpp"
#include "core/Manager.hpp"
#include "util/Debugger.hpp"

#include <SFML/Graphics.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <string_view>
#include <cstdint>

constexpr std::string_view WINDOW_TITLE = "art-gallery-ghost";
constexpr std::uint8_t FPS = 60;

using namespace mir;

Game::Game() {
    Debugger::Log("Game Initialized!");
    initWindow();
}

void Game::initWindow(){
    sf::RenderWindow& window = Manager::GetInstance().Window;

    const sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    window = sf::RenderWindow(
        desktopMode,
        WINDOW_TITLE.data(),
        sf::Style::Default);

    window.setFramerateLimit(FPS);
}

void Game::Run() {
    sf::RenderWindow& window = Manager::GetInstance().Window;
    sf::Clock clock;

    while(window.isOpen()){
        const float deltaTime = clock.restart().asSeconds();
        Manager::ProcessEvent(window);

        window.clear(sf::Color::Black);
        update(deltaTime);
        window.display();
    }
}

void Game::update(const float deltaTime){
    for(ID id = 0; id < MAX_ENTITIES; ++id){
        movement::Update(id, deltaTime);
        render::Update(id, deltaTime);
    }
}
