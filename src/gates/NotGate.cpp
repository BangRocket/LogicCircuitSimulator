#include "NotGate.h"
#include "../utils/ResourceManager.h"
#include <iostream>

NotGate::NotGate(Vector2 position) : Component(position, "not_gate", 1, 1) {
    // Load the SVG texture for the NOT gate
    ResourceManager::getInstance().loadSVGTexture("not_gate", "assets/not_gate.svg", 100, 100);
    std::cout << "NOT gate created at position: (" << position.x << ", " << position.y << ")" << std::endl;
}

void NotGate::Update() {
    // Perform NOT operation
    bool result = !GetInputState(0);
    outputStates[0] = result;
}

void NotGate::Draw() const {
    Texture2D texture = ResourceManager::getInstance().getTexture("not_gate");
    if (texture.id != 0) {
        DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
    } else {
        std::cerr << "Failed to get NOT gate texture for drawing" << std::endl;
        // Fallback drawing if texture is not available
        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), 100, 100, BLUE);
    }
    
    // Call the base class Draw method to render pins
    Component::DrawPins();
}

Vector2 NotGate::GetInputPinPosition(int index) const {
    return { position.x, position.y + 50 };
}

Vector2 NotGate::GetOutputPinPosition(int index) const {
    return { position.x + 100, position.y + 50 };
}