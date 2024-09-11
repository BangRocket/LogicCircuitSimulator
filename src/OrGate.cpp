#include "OrGate.h"

OrGate::OrGate(Vector2 position) : Component(position, "or_gate", 2, 1) {}

void OrGate::Update() {
    // Perform OR operation
    bool result = GetInputState(0) || GetInputState(1);
    outputStates[0] = result;
}