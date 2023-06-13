#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

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

    static void AddTile(int id, int x, int y);
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent*> colliders;

    bool running() { return isRunning; };

private:
    bool isRunning;
    SDL_Window *window;
    int cnt = 0;
};

#endif /* Game_hpp */