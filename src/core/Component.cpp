#include "Component.h"
#include "../managers/ResourceManager.h"
#include "../managers/ConnectionManager.h"
#include "raymath.h"

const float Component::PIN_RADIUS = 5.0f;
const float Component::PIN_HOVER_RADIUS = 10.0f;

Component::Component(Vector2 position, const std::string& textureKey, int numInputs, int numOutputs)
    : position(position), textureKey(textureKey), numInputs(numInputs), numOutputs(numOutputs), isHighlighted(false)
{
    inputStates.resize(numInputs, false);
    outputStates.resize(numOutputs, false);
}

void Component::Draw() const
{
    Texture2D texture = ResourceManager::getInstance().getTexture(textureKey);
    Color tint = isHighlighted ? YELLOW : WHITE;
    DrawTextureV(texture, position, tint);
    DrawPins();

    if (isHighlighted) {
        Rectangle bounds = {position.x, position.y, (float)texture.width, (float)texture.height};
        DrawRectangleLinesEx(bounds, 2, YELLOW);
    }
}

Vector2 Component::GetInputPinPosition(int index) const
{
    // Default implementation, can be overridden by derived classes
    return { position.x, position.y + 30.0f + index * 40.0f };
}

Vector2 Component::GetOutputPinPosition(int index) const
{
    // Default implementation, can be overridden by derived classes
    return { position.x + 100.0f, position.y + 50.0f };
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
        DrawCircle(pinPos.x, pinPos.y, PIN_RADIUS, inputStates[i] ? RED : BLACK);
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetOutputPinPosition(i);
        DrawCircle(pinPos.x, pinPos.y, PIN_RADIUS, outputStates[i] ? RED : BLACK);
    }
}

bool Component::IsHovered(Vector2 mousePosition) const
{
    Texture2D texture = ResourceManager::getInstance().getTexture(textureKey);
    Rectangle componentRect = { position.x, position.y, (float)texture.width, (float)texture.height };
    return CheckCollisionPointRec(mousePosition, componentRect);
}

int Component::HoveredPin(Vector2 mousePosition) const
{
    for (int i = 0; i < numInputs; ++i)
    {
        Vector2 pinPos = GetInputPinPosition(i);
        if (CheckCollisionPointCircle(mousePosition, pinPos, PIN_HOVER_RADIUS))
        {
            return i;
        }
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetOutputPinPosition(i);
        if (CheckCollisionPointCircle(mousePosition, pinPos, PIN_HOVER_RADIUS))
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
