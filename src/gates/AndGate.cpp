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
    Vector2 scaledSize = GetScaledSize();
    float scale = scaledSize.x / size.x;
    Vector2 topLeft = {position.x - scaledSize.x / 2, position.y - scaledSize.y / 2};

    DrawRectangleV(topLeft, scaledSize, WHITE);
    DrawRectangleLinesEx({topLeft.x, topLeft.y, scaledSize.x, scaledSize.y}, 2 * scale, BLACK);

    // Draw input pins
    float pinRadius = 5 * scale;
    DrawCircleV({topLeft.x, topLeft.y + scaledSize.y * 0.25f}, pinRadius, BLACK);
    DrawCircleV({topLeft.x, topLeft.y + scaledSize.y * 0.75f}, pinRadius, BLACK);

    // Draw output pin
    DrawCircleV({topLeft.x + scaledSize.x, topLeft.y + scaledSize.y * 0.5f}, pinRadius, BLACK);

    // Draw AND text
    int fontSize = static_cast<int>(20 * scale);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), "AND", fontSize, 1);
    Vector2 textPos = {
        position.x - textSize.x / 2,
        position.y - textSize.y / 2
    };
    DrawTextEx(GetFontDefault(), "AND", textPos, fontSize, 1, BLACK);
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