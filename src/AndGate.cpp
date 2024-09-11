#include "AndGate.h"

AndGate::AndGate(Vector2 position) : Component(position, "and_gate", 2, 1) {}

void AndGate::Update() {
    // Perform AND operation
    bool result = GetInputState(0) && GetInputState(1);
    outputStates[0] = result;
}