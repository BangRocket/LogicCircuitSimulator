#include "ComponentManager.h"
#include "../core/Component.h"
#include <algorithm>

ComponentManager& ComponentManager::getInstance() {
    static ComponentManager instance;
    return instance;
}

void ComponentManager::addComponent(Component* component) {
    components.push_back(component);
    component->SetComponentManager(this);
}

void ComponentManager::removeComponent(Component* component) {
    components.erase(std::remove(components.begin(), components.end(), component), components.end());
}

void ComponentManager::setInitialScreenSize(int width, int height) {
    initialScreenWidth = width;
    initialScreenHeight = height;
}

void ComponentManager::updateComponentPositions(int newWidth, int newHeight, int originalWidth, int originalHeight) {
    float scaleX = static_cast<float>(newWidth) / initialScreenWidth;
    float scaleY = static_cast<float>(newHeight) / initialScreenHeight;
    float scale = std::min(scaleX, scaleY);

    SetScale(scale);

    for (auto& component : components) {
        Vector2 originalPos = component->GetPosition();
        Vector2 relativePos = {
            originalPos.x / initialScreenWidth,
            originalPos.y / initialScreenHeight
        };
        Vector2 newPos = {
            relativePos.x * newWidth,
            relativePos.y * newHeight
        };
        component->SetPosition(newPos);
        component->SetScale(scale);
    }

    initialScreenWidth = newWidth;
    initialScreenHeight = newHeight;
}

std::vector<Component*>& ComponentManager::getComponents() {
    return components;
}
