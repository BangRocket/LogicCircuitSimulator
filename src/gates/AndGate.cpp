#include "AndGate.h"
#include "../utils/ResourceManager.h"
#include <iostream>

AndGate::AndGate(Vector2 position) : Component(position, "and_gate", 2, 1) {
    // Load the SVG texture for the AND gate
    ResourceManager::getInstance().loadSVGTexture("and_gate", "assets/and_gate.svg", 100, 100);
    std::cout << "AND gate created at position: (" << position.x << ", " << position.y << ")" << std::endl;
}

void AndGate::Update() {
    // Perform AND operation
    bool result = GetInputState(0) && GetInputState(1);
    outputStates[0] = result;
}

void AndGate::Draw() const {
    Texture2D texture = ResourceManager::getInstance().getTexture("and_gate");
    if (texture.id != 0) {
        DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
    } else {
        std::cerr << "Failed to get AND gate texture for drawing" << std::endl;
        // Fallback drawing if texture is not available
        DrawRectangle(static_cast<int>(position.x), static_cast<int>(position.y), 100, 100, RED);
    }
    
    // Call the base class Draw method to render pins
    Component::DrawPins();
}

Vector2 AndGate::GetInputPinPosition(int index) const {
    if (index == 0) {
        return { position.x, position.y + 30 };
    } else {
        return { position.x, position.y + 70 };
    }
}

Vector2 AndGate::GetOutputPinPosition(int index) const {
    return { position.x + 100, position.y + 50 };
}