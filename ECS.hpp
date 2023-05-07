#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

// for both ComponentID and getComponentID
// When the getComponentTypeID function template is called for the first time with a given type T, the static local variable typeID is initialized with the result of calling ComponentTypeID(). This function simply returns a new value of type ComponentID by incrementing a static local variable lastID.
// In subsequent calls to getComponentTypeID with the same type T, the static local variable typeID has already been initialized with the correct unique type ID, so the function simply returns the previously assigned value.
// In your example, the getComponentTypeID function is called twice for type A and twice for type B. However, because the typeID variable is declared as static, its value persists between function calls. This means that the second call to getComponentTypeID<B>() and the second call to getComponentTypeID<A>() simply return the previously assigned type IDs for those types, rather than generating new ones. Therefore, the typeID remains the same for A and B in your example.

// The purpose of these both funcs used with ComponentBitSet and ComponentArray are used to check if a certain entity has a component type
// and if so we can retrive it quickly and do smth with it

inline ComponentID ComponentID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = ComponentID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

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
    bool active = true;
    // initaite a class with unique pointer in vector
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;

public:
    void update()
    {
        for(auto& c : components) c->update();
        for(auto& c : components) c->draw();
    }

    void draw() {}
    bool isActive() const { return active; }
    void destroy() { active = false; }

    template <typename T> bool hasComponent() const
    {
        // if its a template function then have to put <T>
        return componentBitSet[getComponentTypeID<T>() ];
    }

    // In this case, TArgs represents the pack of function arguments with unspecified types can pe passed into the function
    // the && means the it can be rvalue or lvalue
    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        // we begin by allocating the component of type 'T'
        // on the heap, by forwarding the passed arguments
        // to its constructor.
        // notes the std::forward is a template function to move
        // the type arguments pack to another variable/func/stuff
        T* c(new T(std::forward<TArgs>(mArgs)...));
        
        // then set component's entity to the current instance
        c->entity = this;
        
        // wrap the pointer into smart pointer because
        // have to store it back to the container hence
        // we have to follow the same datatype
        std::unique_ptr<Component> uPtr{ c };
        
        // ultimately store in back into the container (components)
        // with using std::move cause unique_ptr cannot be copied so
        // we move it directly
        components.emplace_back(std::move(uPtr));

        // if you put the parenthesis in this situation means its gonna call the function
        // By this one we can store every entity constructor automatically while addind new component
        // instead of storing them manually
        // Also the component Array store all the unique type and bitset make sure it has
        // something on the position
        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;
  
        c->init();
        return *c;
    }

    template<typename T> T& getComponent() const
    {
        auto componentPtr = componentArray[getComponentTypeID<T>()];
        T* ptr;
        ptr = static_cast<T*>(componentPtr);
        return *ptr; // dereference it
    }
    // example of using the component system
    // gameobject.getComponent<PositionComponent>().setXpos(25);

};

class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;

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
        // start by cleaning up the 'dead' entity
        // after cleaning up make sure to update again
        entities.erase(std::remove_if(std::begin(entities), std::end(entities), 
            [](const std::unique_ptr<Entity> &mEntity)
        {
            // return ture if its not active
            return !mEntity->isActive();
        }),
            std::end(entities));
    }
    Entity& addEntity()
    {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
};
