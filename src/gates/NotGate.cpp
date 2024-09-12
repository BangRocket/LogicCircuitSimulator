#include "NotGate.h"
#include "../managers/ResourceManager.h"
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
    Texture2D texture = GetTexture();
    Color tint = isHighlighted ? YELLOW : WHITE;
    Vector2 scaledSize = GetScaledSize();
    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, scaledSize.x, scaledSize.y };
    Vector2 origin = { scaledSize.x / 2, scaledSize.y / 2 };
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
    DrawPins();

    if (isHighlighted) {
        DrawRectangleLinesEx(dest, 2, YELLOW);
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
    return { position.x, position.y + 50 };
}

Vector2 NotGate::GetOutputPinPosition(int index) const {
    return { position.x + 100, position.y + 50 };
}
