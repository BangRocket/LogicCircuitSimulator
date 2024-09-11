#include "InputSwitch.h"
#include "../managers/ResourceManager.h"
#include <iostream>

const float InputSwitch::SWITCH_RADIUS = 15.0f;

InputSwitch::InputSwitch(Vector2 position) : Component(position, "input_switch", 0, 1), state(false) {
    std::cout << "InputSwitch created at position: (" << position.x << ", " << position.y << ")" << std::endl;
}

void InputSwitch::Update() {
    SetOutputState(0, state);
}

void InputSwitch::Draw() const {
    Vector2 scaledSize = GetScaledSize();
    Vector2 topLeft = {position.x - scaledSize.x / 2, position.y - scaledSize.y / 2};

    DrawRectangleV(topLeft, scaledSize, WHITE);
    DrawRectangleLinesEx({topLeft.x, topLeft.y, scaledSize.x, scaledSize.y}, 2 * scale, BLACK);

    // Draw output pin
    Vector2 pinPos = GetOutputPinPosition(0);
    DrawCircleV(pinPos, PIN_RADIUS * scale, BLACK);

    // Draw switch state
    Color switchColor = state ? GREEN : RED;
    DrawRectangle(topLeft.x + scaledSize.x * 0.25f, topLeft.y + scaledSize.y * 0.25f,
                  scaledSize.x * 0.5f, scaledSize.y * 0.5f, switchColor);

    // Draw INPUT text
    int fontSize = static_cast<int>(16 * scale);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), "INPUT", fontSize, 1);
    Vector2 textPos = {
        position.x - textSize.x / 2,
        topLeft.y + scaledSize.y + 5 * scale
    };
    DrawTextEx(GetFontDefault(), "INPUT", textPos, fontSize, 1, BLACK);
}

bool InputSwitch::IsHovered(Vector2 mousePosition) {
	Vector2 scaledSize = GetScaledSize();
	float scale = scaledSize.x / size.x;
	Vector2 topLeft = {position.x - scaledSize.x / 2, position.y - scaledSize.y / 2};
    return CheckCollisionPointRec(mousePosition, {topLeft.x, topLeft.y, scaledSize.x, scaledSize.y});
}

void InputSwitch::ToggleState() {
    state = !state;
    std::cout << "InputSwitch state toggled to: " << (state ? "ON" : "OFF") << std::endl;
}

Vector2 InputSwitch::GetOutputPinPosition(int index) const {
    if (index == 0) {
        Vector2 pos = GetPosition();
        return {pos.x + SWITCH_RADIUS * 2, pos.y + SWITCH_RADIUS};
    }
    return {0, 0}; // Invalid index
}
