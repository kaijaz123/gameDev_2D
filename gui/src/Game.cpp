#include "Game.hpp"
#include "GameObject.hpp"
#include "ECS/Components.hpp"
#include "Map.hpp"
#include "Vector2D.hpp"
// #include "spdlog/spdlog.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Map* map;
Manager manager;

std::vector<ColliderComponent*> Game::colliders;

// Auto keywords here means its not pointer type
// hence we can call the func by object.func()
// unless we specify catch with a pointer then it becomes
// object->func()
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

Game::Game()
{

}

Game::~Game()
{

}

void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen) { flags = SDL_WINDOW_FULLSCREEN; } // if fullscreen

    // Window Initialization
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        // spdlog::info("Subsystems Initialized!");
        std::cout << "Subsystems Initialized!" << std::endl;
        
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) { std::cout << "Windows created!" << std::endl;; }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) 
            { 
                std::cout << "Rerender Created!" << std::endl;
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

        isRunning = true;
    }
    map = new Map();

    // ecs implementation
    Map::LoadMap("gui/assets/map16x16.map", 16, 16);

    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("gui/assets/player.png");
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");

    wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
    wall.addComponent<SpriteComponent>("gui/assets/dirt.png");
    wall.addComponent<ColliderComponent>("wall");
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();

    for (auto cc: colliders)
    {
        Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
    }
}

void Game::render()
{
    SDL_RenderClear(renderer); // Clear the screen to render
    

    manager.draw();
    SDL_RenderPresent(renderer); // Start the rendering
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}

void Game::AddTile(int id, int x, int y)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 32, 32, id);
}