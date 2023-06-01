#pragma once

#include "Components.hpp"
#include "SDL2/SDL.h"

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

public:
    SpriteComponent() = default;
    SpriteComponent(const char* path)
    {   
        setTex(path);
    }

    void setTex(const char* path)
    {
        texture = TextureManager::LoadTexture(path);
    }

    void init() override
    {
        // Look at the getComponent return value
        // It returns the dereference item (the value) hence we cant receive it directly else will get conversion error
        // so thats why the & is added here means we capture the address here then access it with -> later
        // Derefence means e.g. *pointer = &num then we receive it with the & symbol then access the value later on
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = srcRect.h = 32;
        destRect.w = destRect.h = 64;
    }

    void update() override
    {
        destRect.x = (int)transform->position.x;
        destRect.y = (int)transform->position.y;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
};