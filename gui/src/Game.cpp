#include "Game.hpp"
#include "GameObject.hpp"
#include "ECS/Components.hpp"
#include "Map.hpp"
#include "Vector2D.hpp"
#include "AssetManager.hpp"
// #include "spdlog/spdlog.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = {0,0,800,640};
Map* map;
Manager manager;
AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

// Auto keywords here means its not pointer type
// hence we can call the func by object.func()
// unless we specify catch with a pointer then it becomes
// object->func()
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

    assets->AddTexture("terrain", "gui/assets/terrain_ss.png");
    assets->AddTexture("player", "gui/assets/player_anis.png");
    assets->AddTexture("projectiles", "gui/assets/proj.png");

    map = new Map("terrain", 3, 32);

    // ecs implementation
    map->LoadMap("gui/assets/map.map", 25, 20); // map is already fully renderered at the first place

    player.addComponent<TransformComponent>(800, 640, 32, 32, 4);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    assets->CreateProjectile(Vector2D(600, 600), Vector2D(2,0), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(600, 620), Vector2D(2,1), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(400, 600), Vector2D(2,-1), 200, 2, "projectile");
    assets->CreateProjectile(Vector2D(600, 600), Vector2D(2,0), 200, 2, "projectile");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

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
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    manager.refresh();
    manager.update();

    // Vector2D pVel = player.getComponent<TransformComponent>().velocity;
    // int pSpeed = player.getComponent<TransformComponent>().speed;

    // for (auto cc: colliders)
    // {
    //     Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
    // }
    // for (auto t : tiles)
    // {
    //     // Just need to change the destRect because everything things are
    //     // already rendered at the game initialization
    //     // So when anytime we wanna move something we just move the destRect
    //     // *destRect means which position of the destination is gonna get shown
    //     // in our game window
    //     // Lets say have 1024x1024 (all map) and current window size is 240x240
    //     // So in this case if we move our player and the position on map
    //     // we just need to change to according player and map destRect position
    //     t->getComponent<TileComponent>().destRect.x += -(pVel.x * pSpeed);
	// 	   t->getComponent<TileComponent>().destRect.y += -(pVel.y * pSpeed);
    // }
    for (auto& c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(cCol, playerCol))
        {
            std::cout << "Hit" << std::endl;
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }

    for (auto& p : projectiles)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, 
                            p->getComponent<ColliderComponent>().collider))
        {
            std::cout << "Hit player" << std::endl;
            p->destroy();
        }
    }

    camera.x = player.getComponent<TransformComponent>().position.x - 400;
    camera.y = player.getComponent<TransformComponent>().position.y - 320;
    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > camera.w)
        camera.x = camera.w;
    if (camera.y > camera.h)
        camera.y = camera.h;
}

void Game::render()
{
    SDL_RenderClear(renderer); // Clear the screen to render
    for (auto& t : tiles)
    {
        t->draw();
    }

    for (auto& c : colliders)
    {
        c->draw();
    }

    for (auto& p : players)
    {
        p->draw();
    }

    for (auto& p : projectiles)
    {
        p->draw();
    }

    SDL_RenderPresent(renderer); // Start the rendering
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!" << std::endl;
}