#pragma once

#include <vector>
#include "raylib.h"

// Forward declaration
class Component;

class ComponentManager {
public:
    static ComponentManager& getInstance();

    void addComponent(Component* component);
    void removeComponent(Component* component);
    void updateComponentPositions(int newWidth, int newHeight, int originalWidth, int originalHeight);
    std::vector<Component*>& getComponents();
    void setInitialScreenSize(int width, int height);

    float GetScale() const { return scale; }
    void SetScale(float newScale) { scale = newScale; }

private:
    ComponentManager() = default;
    ~ComponentManager() = default;
    ComponentManager(const ComponentManager&) = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;

    std::vector<Component*> components;
    int originalWidth = 800;
    int originalHeight = 600;
    int initialScreenWidth;
    int initialScreenHeight;
    float scale = 1.0f; // Default scale
};