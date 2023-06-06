#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "Components.hpp"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    std::string tag;
    // For the case where object A collides into object B,
    // the information of object B is represented by the tag (i.e. a player, a house, etc.)

    TransformComponent *transform;
    ColliderComponent(std::string t)
    {
        tag = t;
    }

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        collider.x = static_cast<int>(transform->position.x);
        collider.y = static_cast<int>(transform->position.y);
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }
};