#include "Game.hpp"
#include "TextureManager.hpp"
#include "GameObject.hpp"
#include "Map.hpp"

#include "ECS.hpp"
#include "Components.hpp"

GameObject* player;
GameObject* enemy;
SDL_Renderer* Game::renderer = nullptr;
Map* map;
Manager manager;
auto& newPlayer(manager.addEntity());

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
        spdlog::info("Subsystems Initialized!");
        
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) { spdlog::info("Window created!"); }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) 
            { 
                spdlog::info("Renderer created!"); 
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }

        isRunning = true;
    }
    player = new GameObject("gui/assets/player.png", 0, 0);
    enemy = new GameObject("gui/assets/enemy.png", 50, 50);
    map = new Map();

    newPlayer.addComponent<PositionComponent>();
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) { isRunning = false;}
    
}

void Game::update()
{
    player->Update();
    enemy->Update();
    manager.update();
    std::cout << newPlayer.getComponent<PositionComponent>().x() << "," <<
                 newPlayer.getComponent<PositionComponent>().y() << std::endl;
}

void Game::render()
{
    SDL_RenderClear(renderer); // Clear the screen to render
    map->DrawMap();
    player->Render();
    enemy->Render();
    SDL_RenderPresent(renderer); // Start the rendering
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    spdlog::info("Game Cleaned!");
}