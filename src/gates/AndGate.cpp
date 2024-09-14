#include "AndGate.h"
#include "../managers/ResourceManager.h"
#include <iostream>
#include <raymath.h>

AndGate::AndGate(Vector2 position) : Component(position, "and_gate", 2, 1) {
    // Load the SVG texture for the AND gate
    ResourceManager::getInstance().loadSVGTexture("and_gate", "assets/and_gate.svg", 200, 200);
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
    Vector2 origin = { scaledSize.x / 2, scaledSize.y / 2 };  // Set origin to center of texture
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
    
    // Draw a smaller version of the texture
    float smallerScale = 0.8f;  // Adjust this value to make the gate smaller within the 2x2 grid
    Rectangle smallerDest = {
        position.x,
        position.y,
        scaledSize.x * smallerScale,
        scaledSize.y * smallerScale
    };
    Vector2 smallerOrigin = {
        smallerDest.width / 2,
        smallerDest.height / 2
    };
    DrawTexturePro(texture, source, smallerDest, smallerOrigin, rotation, tint);
    DrawPins();

    if (isHighlighted) {
        DrawRectangleLinesEx(Rectangle{
            position.x - scaledSize.x / 2,
            position.y - scaledSize.y / 2,
            scaledSize.x,
            scaledSize.y
        }, 2, ORANGE);
    }
    DrawDebugFrames();
}

bool AndGate::IsHovered(Vector2 mousePosition) {
    Vector2 scaledSize = GetScaledSize();
    Vector2 topLeft = {position.x - scaledSize.x / 2, position.y - scaledSize.y / 2};
    return CheckCollisionPointRec(mousePosition, {topLeft.x, topLeft.y, scaledSize.x, scaledSize.y});
}

Vector2 AndGate::GetInputPinPosition(int index) const {
    Vector2 localPos;
    if (index == 0) {
        localPos = { -size.x / 2, -size.y / 2 + 30 };
    } else {
        localPos = { -size.x / 2, size.y / 2 - 30 };
    }
    localPos = Vector2Scale(localPos, scale);
    localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
    return Vector2Add(position, localPos);
}

Vector2 AndGate::GetOutputPinPosition(int index) const {
    Vector2 localPos = { size.x / 2, 0 };
    localPos = Vector2Scale(localPos, scale);
    localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
    return Vector2Add(position, localPos);
}
