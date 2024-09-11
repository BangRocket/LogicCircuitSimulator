#include "Component.h"
#include "../utils/ResourceManager.h"

Component::Component(Vector2 position, const std::string& textureKey, int numInputs, int numOutputs)
    : position(position), textureKey(textureKey), numInputs(numInputs), numOutputs(numOutputs)
{
    inputStates.resize(numInputs, false);
    outputStates.resize(numOutputs, false);
}

void Component::Draw() const
{
    Texture2D texture = ResourceManager::getInstance().getTexture(textureKey);
    DrawTexture(texture, position.x, position.y, WHITE);
    DrawPins();
}

Vector2 Component::GetPosition() const
{
    return position;
}

void Component::SetPosition(Vector2 newPosition)
{
    position = newPosition;
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

void Component::DrawPins() const
{
    for (int i = 0; i < numInputs; ++i)
    {
        Vector2 pinPos = GetInputPinPosition(i);
        DrawCircle(pinPos.x, pinPos.y, 5, inputStates[i] ? RED : BLACK);
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetOutputPinPosition(i);
        DrawCircle(pinPos.x, pinPos.y, 5, outputStates[i] ? RED : BLACK);
    }
}
