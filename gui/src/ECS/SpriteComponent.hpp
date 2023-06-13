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

    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
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
        // E.g. *pointer = &value so we follow the exact same format in this case equals *transform = &entity..
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.h = transform->height;
        srcRect.w = transform->width;
    }

    void update() override
    {
        destRect.x = static_cast<int>(transform->position.x);
        destRect.y = static_cast<int>(transform->position.y);
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect);
    }
};