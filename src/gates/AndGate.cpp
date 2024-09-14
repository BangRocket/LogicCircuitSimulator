#include "AndGate.h"
#include "../managers/ResourceManager.h"
#include <iostream>
#include <raymath.h>

AndGate::AndGate(Vector2 position) : Component(position, "and_gate", 2, 1) {
    // Load the SVG texture for the AND gate
    ResourceManager::getInstance().loadSVGTexture("and_gate", "assets/and_gate.svg", 200, 200);
    std::cout << "AND gate created at position: (" << position.x << ", " << position.y << ")" << std::endl;

    // Set pin positions based on the SVG
    inputPins[0] = {-1.0f, -0.4f};
    inputPins[1] = {-1.0f, 0.4f};
    outputPins[0] = {1.0f, 0.0f};
}

void AndGate::Update() {
    // Perform AND operation
    bool result = GetInputState(0) && GetInputState(1);
    outputStates[0] = result;
}

void AndGate::Draw() const {
    DrawComponent();
    DrawPins();
    DrawDebugFrames();
}

bool AndGate::IsHovered(Vector2 mousePosition) {
    float hoverRadius = 10.0f * scale; // Adjust this value to change the size of the hover area
    return CheckCollisionPointCircle(mousePosition, position, hoverRadius);
}

Vector2 AndGate::GetInputPinPosition(int index) const {
    Vector2 localPos;
    if (index == 0) {
        localPos = { -size.x / 2, -size.y / 2 + 30 };
    } else {
        localPos = { -size.x / 2, size.y / 2 - 30 };
    }
    localPos = Vector2Scale(localPos, scale);
    localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
    return Vector2Add(position, localPos);
}

Vector2 AndGate::GetOutputPinPosition(int index) const {
    Vector2 localPos = { size.x / 2, 0 };
    localPos = Vector2Scale(localPos, scale);
    localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
    return Vector2Add(position, localPos);
}
