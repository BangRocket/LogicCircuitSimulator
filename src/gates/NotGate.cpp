#include "NotGate.h"
#include "../managers/ResourceManager.h"
#include <iostream>
#include <raymath.h>

NotGate::NotGate(Vector2 position) : Component(position, "not_gate", 1, 1) {
    // Load the SVG texture for the NOT gate
    ResourceManager::getInstance().loadSVGTexture("not_gate", "assets/not_gate.svg", 200, 200);
    std::cout << "NOT gate created at position: (" << position.x << ", " << position.y << ")" << std::endl;
}

void NotGate::Update() {
    // Perform NOT operation
    bool result = !GetInputState(0);
    outputStates[0] = result;
}

void NotGate::Draw() const {
    Texture2D texture = GetTexture();
    Color tint = isHighlighted ? YELLOW : WHITE;
    Vector2 scaledSize = GetScaledSize();
    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, scaledSize.x, scaledSize.y };
    Vector2 origin = { scaledSize.x / 2, scaledSize.y / 2 };  // Set origin to center of texture
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
    DrawPins();

    if (isHighlighted) {
        DrawRectangleLinesEx(Rectangle{
            position.x - scaledSize.x / 2,
            position.y - scaledSize.y / 2,
            scaledSize.x,
            scaledSize.y
        }, 2, ORANGE);
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
