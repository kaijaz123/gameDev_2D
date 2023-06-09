#pragma once
#include "Components.hpp"
#include "../Vector2D.hpp"

class TransformComponent : public Component
{
public:
    Vector2D position;
    Vector2D velocity;

    int height = 32;
    int width = 32;
    int scale = 1;
    int speed = 3;


    TransformComponent()
    {
        // Base constructor
        position.Zero();
    }

    TransformComponent(int sc)
    {
        // Constructor with specified object scale
        position.x = 400;
        position.y = 320;
        scale = sc;
    }

    TransformComponent(float x, float y)
    {
        // Constructor with specified object position
        position.x = x;
        position.y = y;         
    }

    TransformComponent(float x, float y, int h, int w, int sc)
    {
        // Constructor with specified object position, size, scaling
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = sc;
    }

    void init() override
    {
        velocity.Zero();
    }

    void update() override
    {
		position.x += static_cast<int>(velocity.x * speed);
		position.y += static_cast<int>(velocity.y * speed);
    }

};