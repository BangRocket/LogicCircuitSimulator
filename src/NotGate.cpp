#include "NotGate.h"

NotGate::NotGate(Vector2 position) : Component(position, "not_gate", 1, 1) {}

void NotGate::Update() {
    // Perform NOT operation
    bool result = !GetInputState(0);
    outputStates[0] = result;
}