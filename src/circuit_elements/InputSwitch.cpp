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
    Vector2 pos = GetPosition();
    Color switchColor = state ? GREEN : RED;
    Color tint = IsHighlighted() ? YELLOW : WHITE;

    // Draw the switch base
    DrawCircle(pos.x + SWITCH_RADIUS, pos.y + SWITCH_RADIUS, SWITCH_RADIUS, LIGHTGRAY);

    // Draw the switch state
    DrawCircle(pos.x + SWITCH_RADIUS, pos.y + SWITCH_RADIUS, SWITCH_RADIUS * 0.8f, switchColor);

    // Draw the component texture
    DrawTextureV(ResourceManager::getInstance().getTexture(GetTextureKey()), pos, tint);

    // Draw the output pin
    Vector2 pinPos = GetOutputPinPosition(0);
    DrawCircle(pinPos.x, pinPos.y, PIN_RADIUS, outputStates[0] ? RED : BLACK);

    // Draw highlight if necessary
    if (IsHighlighted()) {
        DrawCircleLines(pos.x + SWITCH_RADIUS, pos.y + SWITCH_RADIUS, SWITCH_RADIUS + 2, YELLOW);
    }
    
    // Debug information
    DrawText(TextFormat("InputSwitch at (%.1f, %.1f)", pos.x, pos.y), pos.x, pos.y - 20, 10, BLACK);
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