#include "Component.h"
#include "../managers/ResourceManager.h"
#include "../managers/ConnectionManager.h"
#include "raymath.h"

const float Component::PIN_RADIUS = 5.0f;
const float Component::PIN_HOVER_RADIUS = 10.0f;
bool Component::showDebugFrames = false;

Component::Component(Vector2 position, const std::string& textureKey, int numInputs, int numOutputs)
    : position(position), textureKey(textureKey), numInputs(numInputs), numOutputs(numOutputs), isHighlighted(false), scale(1.0f), rotation(0.0f)
{
    inputStates.resize(numInputs, false);
    outputStates.resize(numOutputs, false);
    
    // Default pin positions (to be overridden by specific components)
    for (int i = 0; i < numInputs; ++i) {
        inputPins.push_back({-1.0f, -0.5f + (1.0f / (numInputs + 1)) * (i + 1)});
    }
    for (int i = 0; i < numOutputs; ++i) {
        outputPins.push_back({1.0f, 0.0f});
    }
}

void Component::DrawDebugFrames() const
{
    if (!showDebugFrames) return;

    Vector2 scaledSize = GetScaledSize();
    Rectangle componentRect = { 
        position.x - scaledSize.x / 2, 
        position.y - scaledSize.y / 2, 
        scaledSize.x, 
        scaledSize.y 
    };
    DrawRectangleLinesEx(componentRect, 2, GREEN);

    // Draw a small box to represent the origin
    float originBoxSize = 8 * scale;  // Increased size for better visibility
    DrawRectangle(
        position.x - originBoxSize / 2,
        position.y - originBoxSize / 2,
        originBoxSize,
        originBoxSize,
        ORANGE
    );

    for (int i = 0; i < numInputs; ++i)
    {
        Vector2 pinPos = GetInputPinPosition(i);
        DrawCircleV(pinPos, PIN_RADIUS * 2 * scale, BLUE);
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetOutputPinPosition(i);
        DrawCircleV(pinPos, PIN_RADIUS * 2 * scale, RED);
    }
}

Vector2 Component::GetInputPinPosition(int index) const
{
    if (index >= 0 && index < inputPins.size()) {
        Vector2 scaledSize = GetScaledSize();
        Vector2 localPos = {inputPins[index].x * scaledSize.x / 2, inputPins[index].y * scaledSize.y};
        localPos = Vector2Scale(localPos, scale);
        localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
        return Vector2Add(position, localPos);
    }
    return position;
}

Vector2 Component::GetOutputPinPosition(int index) const
{
    if (index >= 0 && index < outputPins.size()) {
        Vector2 scaledSize = GetScaledSize();
        Vector2 localPos = {outputPins[index].x * scaledSize.x / 2, outputPins[index].y * scaledSize.y};
        localPos = Vector2Scale(localPos, scale);
        localPos = Vector2Rotate(localPos, rotation * DEG2RAD);
        return Vector2Add(position, localPos);
    }
    return position;
}

Vector2 Component::GetPinPosition(int pinIndex) const
{
    if (pinIndex < numInputs)
    {
        return GetInputPinPosition(pinIndex);
    }
    else
    {
        return GetOutputPinPosition(pinIndex - numInputs);
    }
}

bool Component::GetInputState(int inputIndex) const
{
    if (inputIndex >= 0 && inputIndex < numInputs)
    {
        return inputStates[inputIndex];
    }
    return false;
}

bool Component::GetOutputState(int outputIndex) const
{
    if (outputIndex >= 0 && outputIndex < numOutputs)
    {
        return outputStates[outputIndex];
    }
    return false;
}

void Component::SetInputState(int inputIndex, bool state)
{
    if (inputIndex >= 0 && inputIndex < numInputs)
    {
        inputStates[inputIndex] = state;
    }
}

void Component::SetOutputState(int outputIndex, bool state)
{
    if (outputIndex >= 0 && outputIndex < numOutputs)
    {
        outputStates[outputIndex] = state;
    }
}

void Component::DrawPins() const
{
    Vector2 scaledSize = GetScaledSize();
    Vector2 origin = { scaledSize.x / 2, scaledSize.y / 2 };

    for (int i = 0; i < numInputs; ++i)
    {
        Vector2 pinPos = GetInputPinPosition(i);
        Vector2 rotatedPinPos = Vector2Rotate(Vector2Subtract(pinPos, position), rotation * DEG2RAD);
        rotatedPinPos = Vector2Add(rotatedPinPos, position);
        DrawCircleV(rotatedPinPos, PIN_RADIUS * scale, inputStates[i] ? RED : BLACK);
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetOutputPinPosition(i);
        Vector2 rotatedPinPos = Vector2Rotate(Vector2Subtract(pinPos, position), rotation * DEG2RAD);
        rotatedPinPos = Vector2Add(rotatedPinPos, position);
        DrawCircleV(rotatedPinPos, PIN_RADIUS * scale, outputStates[i] ? RED : BLACK);
    }
}

bool Component::IsHovered(Vector2 mousePosition) const
{
    Vector2 scaledSize = GetScaledSize();
    Rectangle componentRect = { 
        position.x - scaledSize.x / 2, 
        position.y - scaledSize.y / 2, 
        scaledSize.x, 
        scaledSize.y 
    };
    Vector2 rotatedMousePos = Vector2Subtract(mousePosition, position);
    rotatedMousePos = Vector2Rotate(rotatedMousePos, -rotation * DEG2RAD);
    rotatedMousePos = Vector2Add(rotatedMousePos, position);
    return CheckCollisionPointRec(rotatedMousePos, componentRect);
}

int Component::HoveredPin(Vector2 mousePosition) const
{
    Vector2 localMousePos = Vector2Subtract(mousePosition, position);
    localMousePos = Vector2Rotate(localMousePos, -rotation * DEG2RAD);

    for (int i = 0; i < numInputs; ++i)
    {
        Vector2 pinPos = GetInputPinPosition(i);
        if (CheckCollisionPointCircle(localMousePos, pinPos, PIN_HOVER_RADIUS * scale))
        {
            return i;
        }
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetOutputPinPosition(i);
        if (CheckCollisionPointCircle(localMousePos, pinPos, PIN_HOVER_RADIUS * scale))
        {
            return numInputs + i;
        }
    }

    return -1; // No pin hovered
}

Wire* Component::GetWireAtPin(int pinIndex) const
{
    return ConnectionManager::getInstance().getWireAtPin(this, pinIndex);
}

bool Component::CanConnectAtPin(int pinIndex) const
{
    return !GetWireAtPin(pinIndex);
}

void Component::Rotate(float angle)
{
    rotation += angle;
    if (rotation >= 360.0f) rotation -= 360.0f;
    if (rotation < 0.0f) rotation += 360.0f;
}

float Component::GetRotation() const
{
    return rotation;
}

void Component::SetRotation(float newRotation)
{
    rotation = newRotation;
    if (rotation >= 360.0f) rotation -= 360.0f;
    if (rotation < 0.0f) rotation += 360.0f;
}

void Component::SetScale(float newScale)
{
    scale = newScale;
}

Texture2D Component::GetTexture() const
{
    return ResourceManager::getInstance().getTexture(textureKey);
}

Vector2 Component::GetOrigin() const
{
    Vector2 scaledSize = GetScaledSize();
    return { scaledSize.x / 2, scaledSize.y / 2 };
}

Rectangle Component::GetDestRect() const
{
    Vector2 scaledSize = GetScaledSize();
    return { 
        position.x - scaledSize.x / 2, 
        position.y - scaledSize.y / 2, 
        scaledSize.x, 
        scaledSize.y 
    };
}

void Component::DrawComponent() const
{
    Texture2D texture = GetTexture();
    Color tint = isHighlighted ? YELLOW : WHITE;
    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle dest = GetDestRect();
    Vector2 origin = GetOrigin();
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
}
