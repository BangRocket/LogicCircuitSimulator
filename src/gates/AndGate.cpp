#include "AndGate.h"
#include "../managers/ResourceManager.h"
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
    Texture2D texture = GetTexture();
    Color tint = isHighlighted ? YELLOW : WHITE;
    Vector2 scaledSize = GetScaledSize();
    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, scaledSize.x, scaledSize.y };
    Vector2 origin = { scaledSize.x, scaledSize.y};
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
    DrawPins();

    if (isHighlighted) {
        DrawRectangleLinesEx(dest, 2, YELLOW);
    }
}

bool AndGate::IsHovered(Vector2 mousePosition) {
    Vector2 scaledSize = GetScaledSize();
    Vector2 topLeft = {position.x - scaledSize.x / 2, position.y - scaledSize.y / 2};
    return CheckCollisionPointRec(mousePosition, {topLeft.x, topLeft.y, scaledSize.x, scaledSize.y});
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
