#include "OrGate.h"
#include "../utils/ResourceManager.h"
#include <iostream>

OrGate::OrGate(Vector2 position) : Component(position, "or_gate", 2, 1) {
    // Load the SVG texture for the OR gate
    ResourceManager::getInstance().loadSVGTexture("or_gate", "assets/or_gate.svg", 100, 100);
    std::cout << "OR gate created at position: (" << position.x << ", " << position.y << ")" << std::endl;
}

void OrGate::Update() {
    // Perform OR operation
    bool result = GetInputState(0) || GetInputState(1);
    outputStates[0] = result;
}

void OrGate::Draw() const {
    Texture2D texture = ResourceManager::getInstance().getTexture("or_gate");
    if (texture.id != 0) {
        DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
    } else {
        std::cerr << "Failed to get OR gate texture for drawing" << std::endl;
        // Fallback drawing if texture is not available
        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), 100, 100, GREEN);
    }
    
    // Call the base class Draw method to render pins
    Component::DrawPins();
}

Vector2 OrGate::GetInputPinPosition(int index) const {
    if (index == 0) {
        return { position.x, position.y + 30 };
    } else {
        return { position.x, position.y + 70 };
    }
}

Vector2 OrGate::GetOutputPinPosition(int index) const {
    return { position.x + 100, position.y + 50 };
}