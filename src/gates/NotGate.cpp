#include "NotGate.h"
#include "../managers/ResourceManager.h"
#include <iostream>
#include <raymath.h>

NotGate::NotGate(Vector2 position) : Component(position, "not_gate", 1, 1) {
    // Load the SVG texture for the NOT gate
    ResourceManager::getInstance().loadSVGTexture("not_gate", "assets/not_gate.svg", 200, 200);
    std::cout << "NOT gate created at position: (" << position.x << ", " << position.y << ")" << std::endl;

    // Set pin positions based on the SVG
    inputPins[0] = {-1.0f, 0.0f};
    outputPins[0] = {1.0f, 0.0f};
}

void NotGate::Update() {
    // Perform NOT operation
    bool result = !GetInputState(0);
    outputStates[0] = result;
}

void NotGate::Draw() const {
    DrawComponent();
    DrawPins();
    if (isHighlighted) {
        DrawRectangleLinesEx(GetDestRect(), 2, ORANGE);
    }
    DrawDebugFrames();
}

bool NotGate::IsHovered(Vector2 mousePosition) {
	Vector2 scaledSize = GetScaledSize();
	Vector2 topLeft = {position.x - scaledSize.x / 2, position.y - scaledSize.y / 2};
    return CheckCollisionPointTriangle(mousePosition,
        {topLeft.x, topLeft.y},
        {topLeft.x, topLeft.y + scaledSize.y},
        {topLeft.x + scaledSize.x, topLeft.y + scaledSize.y / 2}
    );
}

Vector2 NotGate::GetInputPinPosition(int index) const {
    Vector2 localPos = { -size.x / 2, 0 };
    localPos = Vector2Scale(localPos, scale);
    localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
    return Vector2Add(position, localPos);
}

Vector2 NotGate::GetOutputPinPosition(int index) const {
    Vector2 localPos = { size.x / 2, 0 };
    localPos = Vector2Scale(localPos, scale);
    localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
    return Vector2Add(position, localPos);
}
