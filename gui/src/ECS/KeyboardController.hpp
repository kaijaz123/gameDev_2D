#pragma once

#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"

// typedef decltype(Game::event.key.keysym.sym) Key;

class KeyboardController : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        if (Game::event.type == SDL_KEYDOWN)
        {
            if (Game::event.key.keysym.sym == SDLK_w) 
                { transform->velocity.y = -1;
                  sprite->Play("Walk");}

            else if (Game::event.key.keysym.sym == SDLK_s) 
                { transform->velocity.y = 1; 
                  sprite->Play("Walk");}

            else if (Game::event.key.keysym.sym == SDLK_a) 
                { transform->velocity.x = -1; 
                  sprite->Play("Walk");
                  sprite->spriteFlip = SDL_FLIP_HORIZONTAL;}

            else if (Game::event.key.keysym.sym == SDLK_d) 
                { transform->velocity.x = 1; 
                  sprite->Play("Walk");}
        }

        if(Game::event.type == SDL_KEYUP)
        {
            if (Game::event.key.keysym.sym == SDLK_w) 
                { transform->velocity.y = 0; 
                  sprite->Play("Idle");}

            else if (Game::event.key.keysym.sym == SDLK_s) 
                { transform->velocity.y = 0; 
                  sprite->Play("Idle");}

            else if (Game::event.key.keysym.sym == SDLK_a) 
                { transform->velocity.x = 0; 
                  sprite->Play("Idle");
                  sprite->spriteFlip = SDL_FLIP_NONE;}

            else if (Game::event.key.keysym.sym == SDLK_d) 
                { transform->velocity.x = 0; 
                  sprite->Play("Idle");}
            
             else if (Game::event.key.keysym.sym == SDLK_ESCAPE) 
                { Game::isRunning = false;}           
        }
    }
};