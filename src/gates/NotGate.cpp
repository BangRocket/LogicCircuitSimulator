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
	Vector2 scaledSize = GetScaledSize();
	float scale = scaledSize.x / size.x;
	Vector2 topLeft = {position.x - scaledSize.x / 2, position.y - scaledSize.y / 2};

    DrawTriangle(
        {topLeft.x, topLeft.y},
        {topLeft.x, topLeft.y + scaledSize.y},
        {topLeft.x + scaledSize.x, topLeft.y + scaledSize.y / 2},
        WHITE
    );
    DrawTriangleLines(
        {topLeft.x, topLeft.y},
        {topLeft.x, topLeft.y + scaledSize.y},
        {topLeft.x + scaledSize.x, topLeft.y + scaledSize.y / 2},
        BLACK
    );

    // Draw input pin
    float pinRadius = 5 * scale;
    DrawCircleV({topLeft.x, topLeft.y + scaledSize.y * 0.5f}, pinRadius, BLACK);

    // Draw output pin
    DrawCircleV({topLeft.x + scaledSize.x + pinRadius * 2, topLeft.y + scaledSize.y * 0.5f}, pinRadius, BLACK);

    // Draw NOT text
    int fontSize = static_cast<int>(20 * scale);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), "NOT", fontSize, 1);
    Vector2 textPos = {
        position.x - textSize.x / 2,
        position.y - textSize.y / 2
    };
    DrawTextEx(GetFontDefault(), "NOT", textPos, fontSize, 1, BLACK);
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