#include "InputSwitch.h"
#include "../managers/ResourceManager.h"
#include <iostream>
#include <raymath.h>

const float InputSwitch::SWITCH_RADIUS = 10.0f;

InputSwitch::InputSwitch(Vector2 position) : Component(position, "input_switch", 0, 1), state(false) {
    std::cout << "InputSwitch created at position: (" << position.x << ", " << position.y << ")" << std::endl;
}

void InputSwitch::Update() {
    SetOutputState(0, state);
}

void InputSwitch::Draw() const {
    DrawComponent();
    DrawPins();
    DrawDebugFrames();

    // Draw switch state
    Color switchColor = state ? GREEN : RED;
    Vector2 switchPos = { position.x, position.y };
    DrawCircleV(switchPos, SWITCH_RADIUS * scale, switchColor);
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
        Vector2 localPos = { size.x / 2, 0 };
        localPos = Vector2Scale(localPos, scale);
        localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
        return Vector2Add(position, localPos);
    }
    return {0, 0}; // Invalid index
}
