#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

// typedef decltype(Game::event.key.keysym.sym) Key;

class KeyboardController : public Component
{
public:
    TransformComponent *transform;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            if (Game::event.key.keysym.sym == SDLK_w) { transform->velocity.y = -1; }
            else if (Game::event.key.keysym.sym == SDLK_s) { transform->velocity.y = 1; }
            else if (Game::event.key.keysym.sym == SDLK_a) { transform->velocity.x = -1; }
            else if (Game::event.key.keysym.sym == SDLK_d) { transform->velocity.x = 1; }
        }

        if(Game::event.type == SDL_KEYUP)
        {
            if (Game::event.key.keysym.sym == SDLK_w) { transform->velocity.y = 0; }
            else if (Game::event.key.keysym.sym == SDLK_s) { transform->velocity.y = 0; }
            else if (Game::event.key.keysym.sym == SDLK_a) { transform->velocity.x = 0; }
            else if (Game::event.key.keysym.sym == SDLK_d) { transform->velocity.x = 0; }
        }
    }
};