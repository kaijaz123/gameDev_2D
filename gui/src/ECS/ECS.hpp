#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

class Component
{
public:
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}
};

class Entity
{
private:
    Manager& manager;
    bool active = true;
    // initaite a class with unique pointer in vector
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitset groupBitset;

public:
    Entity(Manager& mManager) : manager(mManager) {}

    void update()
    {
        for(auto& c : components) c->update();
    }

    void draw() 
    {
        for(auto& c : components) c->draw();
    }

    bool isActive() const { return active; }
    void destroy() { active = false; }

    template <typename T> bool hasComponent() const
    {
        // if its a template function then have to put <T>
        return componentBitSet[getComponentTypeID<T>() ];
    }

    bool hasGroup(Group mGroup)
    {
        return groupBitset[mGroup];
    }

    void addGroup(Group mGroup);
    void delGroup(Group mGroup)
    {
        groupBitset[mGroup] = false;
    }

    // In this case, TArgs represents the pack of function arguments with unspecified types can pe passed into the function
    // the && means the it can be rvalue or lvalue
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        // we begin by allocating the component of type 't'
        // on the heap, by fowarding the passed arguments to its constructor.
        // Notes the std::forward is a template function to move
        // the type arguments back to another variable/func/stuff
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };
        components.emplace_back(std::move(uPtr));

        // if you put the parenthesis in this situation means its gonna call the function
        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;
  
        c->init();
        return *c;
        // More explanation about the template function itself and how it works
        // at the notes.txt
    }

    // return by reference
    template<typename T> T& getComponent() const
    {
        auto componentPtr = componentArray[getComponentTypeID<T>()];
        T* ptr;
        ptr = static_cast<T*>(componentPtr);
        return *ptr; // dereference it -> return the actual value
    }
    // example of using the component system
    // gameobject.getComponent<PositionComponent>().setXpos(25);

};

class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
    void update()
    {
        for (auto& e: entities) e->update();
    }
    void draw()
    {
        for (auto& e: entities) e->draw();
    }
    void refresh()
    {
        for(auto i(0u); i < maxGroups; i++)
        {
            auto& v(groupedEntities[i]);
            v.erase(
                std::remove_if(std::begin(v), std::end(v),
                    [i](Entity* mEntity)
                {
                    return !mEntity->isActive() || !mEntity->hasGroup(i);
                }),
                std::end(v));
        }

        entities.erase(std::remove_if(std::begin(entities), std::end(entities), 
            [](const std::unique_ptr<Entity> &mEntity)
        {
            // return true if its not active
            return !mEntity->isActive();
        }),
            std::end(entities));
    }

    void AddToGroup(Entity* mEntity, Group mGroup)
    {
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    std::vector<Entity*>& getGroup(Group mGroup)
    {
        return groupedEntities[mGroup];
    }

    Entity& addEntity()
    {
        Entity* e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};