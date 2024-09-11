#ifndef COMPONENT_H
#define COMPONENT_H

#include "raylib.h"
#include <string>
#include <vector>

class Component {
public:
    Component(Vector2 position, const std::string& textureKey, int numInputs, int numOutputs);
    virtual ~Component() = default;

    virtual void Update() = 0;
    virtual void Draw() const;

    Vector2 GetPosition() const;
    void SetPosition(Vector2 newPosition);

    virtual Vector2 GetInputPinPosition(int index) const;
    virtual Vector2 GetOutputPinPosition(int index) const;
    Vector2 GetPinPosition(int pinIndex) const;
    bool GetInputState(int inputIndex) const;
    bool GetOutputState(int outputIndex) const;
    void SetInputState(int inputIndex, bool state);

    int GetNumInputs() const { return numInputs; }
    int GetNumOutputs() const { return numOutputs; }

protected:
    Vector2 position;
    std::string textureKey;
    int numInputs;
    int numOutputs;
    std::vector<bool> inputStates;
    std::vector<bool> outputStates;

    void DrawPins() const;
};

#endif // COMPONENT_H
