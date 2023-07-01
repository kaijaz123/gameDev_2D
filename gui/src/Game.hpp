#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

class AssetManager;
class ColliderComponent;

class Game {

public:
    Game(); // constructor
    ~Game(); // desconstructor

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean(); // memory management

    static SDL_Renderer* renderer;
    static SDL_Event event;
    static bool isRunning;
    static SDL_Rect camera;
    static AssetManager* assets;

    bool running() { return isRunning; };
    enum GroupLabels : std::size_t
    {
        groupMap,
        groupPlayers,
        groupColliders,
        groupProjectiles
    };

private:
    SDL_Window *window;
    int cnt = 0;
};

#endif /* Game_hpp */