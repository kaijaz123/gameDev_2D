#include "Game.hpp"
#include "TextureManager.hpp"
#include "GameObject.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
// #include "spdlog/spdlog.h"

SDL_Renderer* Game::renderer = nullptr;
Map* map;
Manager manager;

auto& player(manager.addEntity());

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
    player.addComponent<PositionComponent>();
    player.addComponent<SpriteComponent>("gui/assets/player.png");
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) { isRunning = false;}
    
}

void Game::update()
{
    manager.refresh();
    manager.update();

    if (player.getComponent<PositionComponent>().x() > 100)
    {
        player.getComponent<SpriteComponent>().setTex("gui/assets/enemy.png");
    }
}

void Game::render()
{
    SDL_RenderClear(renderer); // Clear the screen to render
    map->DrawMap();

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