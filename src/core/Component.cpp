#include "Component.h"
#include "../managers/ResourceManager.h"
#include "../managers/ConnectionManager.h"
#include "raymath.h"

const float Component::PIN_RADIUS = 5.0f;
const float Component::PIN_HOVER_RADIUS = 10.0f;

Component::Component(Vector2 position, const std::string& textureKey, int numInputs, int numOutputs)
    : position(position), textureKey(textureKey), numInputs(numInputs), numOutputs(numOutputs), isHighlighted(false), scale(1.0f), rotation(0.0f)
{
    inputStates.resize(numInputs, false);
    outputStates.resize(numOutputs, false);
}

void Component::Draw() const
{
    Texture2D texture = GetTexture();
    Color tint = isHighlighted ? YELLOW : WHITE;
    Vector2 scaledSize = GetScaledSize();
    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, scaledSize.x, scaledSize.y };
    Vector2 origin = { scaledSize.x / 2, scaledSize.y / 2 };
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
    DrawPins();

    if (isHighlighted) {
        DrawRectanglePro(dest, origin, rotation, YELLOW);
    }
}

Vector2 Component::GetScaledSize() const
{
    return { size.x * scale, size.y * scale };
}

Vector2 Component::GetInputPinPosition(int index) const
{
    Vector2 scaledSize = GetScaledSize();
    Vector2 pinPos = { -scaledSize.x / 2, (-scaledSize.y / 2) + (30.0f + index * 40.0f) * scale };
    return Vector2Add(position, Vector2Rotate(pinPos, rotation * DEG2RAD));
}

Vector2 Component::GetOutputPinPosition(int index) const
{
    Vector2 scaledSize = GetScaledSize();
    Vector2 pinPos = { scaledSize.x / 2, (-scaledSize.y / 2) + 50.0f * scale };
    return Vector2Add(position, Vector2Rotate(pinPos, rotation * DEG2RAD));
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
    for (int i = 0; i < numInputs; ++i)
    {
        Vector2 pinPos = GetInputPinPosition(i);
        DrawCircle(pinPos.x, pinPos.y, PIN_RADIUS * scale, inputStates[i] ? RED : BLACK);
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetOutputPinPosition(i);
        DrawCircle(pinPos.x, pinPos.y, PIN_RADIUS * scale, outputStates[i] ? RED : BLACK);
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
    for (int i = 0; i < numInputs; ++i)
    {
        Vector2 pinPos = GetInputPinPosition(i);
        if (CheckCollisionPointCircle(mousePosition, pinPos, PIN_HOVER_RADIUS * scale))
        {
            return i;
        }
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetOutputPinPosition(i);
        if (CheckCollisionPointCircle(mousePosition, pinPos, PIN_HOVER_RADIUS * scale))
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
