#pragma once

#include "Components.hpp"
#include "SDL2/SDL.h"
#include "Animation.hpp"
#include <map>
#include "../AssetManager.hpp"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    // Animation
    bool animated = false;
    int frames = 0;
    int speed = 100;

public:
    int animIndex = 0;
    std::map<const char*, Animation> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    SpriteComponent(std::string id)
    {   
        setTex(id);
    }

    SpriteComponent(std::string id, bool isAnimated)
    {   
        animated = isAnimated;

        Animation idle = Animation(0, 3, 100);
        Animation walk = Animation(1, 8, 100);

        animations.emplace("Idle", idle);
        animations.emplace("Walk", walk);

        Play("Idle");
        setTex(id);
    }

    ~SpriteComponent()
    {
    }

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void init() override
    {
        // Look at the getComponent return value
        // It returns the dereference item (the value) hence we cant receive it directly else will get conversion error
        // so thats why the & is added here means we capture the address here then access it with -> later
        // Derefence means e.g. *pointer = &num then we receive it with the & symbol then access the value later on
        // E.g. *pointer = &value so we follow the exact same format in this case equals *transform = &entity..
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.h = transform->height;
        srcRect.w = transform->width;
    }

    void update() override
    {
        if (animated)
        {
            // The animation file is 128x32
            // SDL_GetTicks() gets the current elapsed time starting from the INIT
            // Since we have 4 animations in this case -> the first animation x is 0, second is 32, 
            // third is 64, and last is 96
            // So the srcRect.x access the pixel of each x position provided and render it accordingly
            // % here make sures it stays within the number of animations we have (4 the case we provided)
            // the dimensions also should be able to be divided (correlated) 128/32 = 4
            // hence the width is used in this case for multiplication
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
        }
        srcRect.y = animIndex * transform->height;
        
        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(const char* animName)
    {
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
};