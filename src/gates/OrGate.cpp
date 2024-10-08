#include "OrGate.h"
#include "../managers/ResourceManager.h"
#include <iostream>
#include <raymath.h>

OrGate::OrGate(Vector2 position) : Component(position, "or_gate", 2, 1) {
    // Load the SVG texture for the OR gate
    ResourceManager::getInstance().loadSVGTexture("or_gate", "assets/or_gate.svg", 200, 200);
    std::cout << "OR gate created at position: (" << position.x << ", " << position.y << ")" << std::endl;

    // Set pin positions based on the SVG
    inputPins[0] = {-1.0f, -0.4f};
    inputPins[1] = {-1.0f, 0.4f};
    outputPins[0] = {1.0f, 0.0f};
}

void OrGate::Update() {
    // Perform OR operation
    bool result = GetInputState(0) || GetInputState(1);
    outputStates[0] = result;
}

void OrGate::Draw() const {
    DrawComponent();
    DrawPins();
    DrawDebugFrames();
}

bool OrGate::IsHovered(Vector2 mousePosition) {
	Vector2 scaledSize = GetScaledSize();
	Vector2 topLeft = {position.x - scaledSize.x / 2, position.y - scaledSize.y / 2};
    return CheckCollisionPointRec(mousePosition, {topLeft.x, topLeft.y, scaledSize.x, scaledSize.y});
}

Vector2 OrGate::GetInputPinPosition(int index) const {
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

Vector2 OrGate::GetOutputPinPosition(int index) const {
    Vector2 localPos = { size.x / 2, 0 };
    localPos = Vector2Scale(localPos, scale);
    localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
    return Vector2Add(position, localPos);
}
