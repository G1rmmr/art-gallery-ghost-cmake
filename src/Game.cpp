#include "Game.hpp"
#include "util/Profiler.hpp"

constexpr std::string_view WINDOW_TITLE = "art-gallery-ghost";
constexpr std::uint8_t FPS = 60;

Game::Game() {
    mir::debug::Log("Game Initialized!");
    initWindow();
}

void Game::initWindow(){
    mir::Window = sf::RenderWindow(
        sf::VideoMode::getDesktopMode(),
        WINDOW_TITLE.data());

    mir::Window.setFramerateLimit(FPS);
}

void Game::Run(){
    sf::Clock clock;

    while(mir::Window.isOpen()){
        const float deltaTime = clock.restart().asSeconds();
        mir::input::Process();
        update(deltaTime);

        PROFILE_SCOPE("Render"){
            mir::Window.clear(sf::Color::Black);
            render();
            mir::Window.display();
        }
    }
}

void Game::update(const float deltaTime){
    PROFILE_SCOPE("Movement"){
        for(mir::ID id = 0; id < mir::MAX_ENTITIES; ++id)
            mir::movement::Update(id, deltaTime);
    }

    PROFILE_SCOPE("Collision"){
        for(mir::ID id = 0; id < mir::MAX_ENTITIES - 1; ++id){
            for(mir::ID other = id + 1; other < mir::MAX_ENTITIES; ++other)
                mir::collision::Update(id, other);
        }
    }

    PROFILE_SCOPE("RenderUpdate"){
        for(mir::ID id = 0; id < mir::MAX_ENTITIES; ++id)
            mir::render::Update(id, deltaTime);
    }
}

void Game::render(){
    for(mir::ID id = 0; id < mir::MAX_ENTITIES; ++id){
        if(!mir::sprite::Textures[id]) continue;

        const std::vector<sf::IntRect>& frames = mir::animation::FrameSets[mir::animation::States[id]];

        sf::Sprite sprite = frames.empty() ?
            sf::Sprite(*mir::sprite::Textures[id]) :
            sf::Sprite(*mir::sprite::Textures[id], frames[mir::animation::CurrFrames[id]]);

        mir::texture::BuildSprite(id, sprite);
        mir::Window.draw(sprite);
    }
}
