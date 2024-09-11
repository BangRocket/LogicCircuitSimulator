#include "Component.h"
#include "ResourceManager.h"

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

Vector2 Component::GetPinPosition(int pinIndex) const
{
    const int PIN_SPACING = 32;
    if (pinIndex < numInputs)
    {
        return { position.x, position.y + (pinIndex + 1) * PIN_SPACING };
    }
    else
    {
        int outputIndex = pinIndex - numInputs;
        return { position.x + 64, position.y + (outputIndex + 1) * PIN_SPACING };
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
        Vector2 pinPos = GetPinPosition(i);
        DrawCircle(pinPos.x, pinPos.y, 4, inputStates[i] ? RED : GRAY);
    }

    for (int i = 0; i < numOutputs; ++i)
    {
        Vector2 pinPos = GetPinPosition(i + numInputs);
        DrawCircle(pinPos.x, pinPos.y, 4, outputStates[i] ? RED : GRAY);
    }
}
