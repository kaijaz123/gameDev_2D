#include <iostream>
#include "Game.hpp"

int WinMain(int argc, char const *argv[])
{
    // Cap our frame rate
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    Game *game = new Game();
    game->init("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, 0);

    while (game->running())
    {
        frameStart = SDL_GetTicks(); // Calculate framerate

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Game ends
    game->clean();
    return 0;
}