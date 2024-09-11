#ifndef COMPONENT_H
#define COMPONENT_H

#include "raylib.h"
#include <string>
#include <vector>

class Wire;
class ConnectionManager;

class Component {
public:
    Component(Vector2 position, const std::string& textureKey, int numInputs, int numOutputs);
    virtual ~Component() = default;

    virtual void Update() = 0;
    virtual void Draw() const;

    Vector2 GetPosition() const { return position; }
    void SetPosition(Vector2 newPosition) { position = newPosition; }

    virtual Vector2 GetInputPinPosition(int index) const;
    virtual Vector2 GetOutputPinPosition(int index) const;
    Vector2 GetPinPosition(int pinIndex) const;
    
    bool GetInputState(int inputIndex) const;
    bool GetOutputState(int outputIndex) const;
    void SetInputState(int inputIndex, bool state);
    void SetOutputState(int outputIndex, bool state);

    int GetNumInputs() const { return numInputs; }
    int GetNumOutputs() const { return numOutputs; }

    bool IsHovered(Vector2 mousePosition) const;
    int HoveredPin(Vector2 mousePosition) const;

    const std::string& GetTextureKey() const { return textureKey; }

    void SetHighlighted(bool highlight) { isHighlighted = highlight; }
    bool IsHighlighted() const { return isHighlighted; }

    Wire* GetWireAtPin(int pinIndex) const;
    bool CanConnectAtPin(int pinIndex) const;

protected:
    Vector2 position;
    std::string textureKey;
    int numInputs;
    int numOutputs;
    std::vector<bool> inputStates;
    std::vector<bool> outputStates;
    bool isHighlighted;

    void DrawPins() const;

    static const float PIN_RADIUS;
    static const float PIN_HOVER_RADIUS;

    friend class ConnectionManager;
};

#endif // COMPONENT_H
