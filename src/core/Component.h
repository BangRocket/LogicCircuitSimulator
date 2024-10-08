#ifndef COMPONENT_H
#define COMPONENT_H

#include "raylib.h"
#include <string>
#include <vector>

class Wire;
class ConnectionManager;
class ComponentManager; // Forward declaration

class Component {
public:
    Component(Vector2 position, const std::string& textureKey, int numInputs, int numOutputs);
    virtual ~Component() = default;

    virtual void Update() = 0;
    virtual void Draw() const = 0;

    Vector2 GetPosition() const { return position; }
    void SetPosition(Vector2 newPosition) { position = newPosition; }

    struct PinPosition {
        float x;
        float y;
    };
    
    virtual Vector2 GetInputPinPosition(int index) const;
    virtual Vector2 GetOutputPinPosition(int index) const;
    Vector2 GetPinPosition(int pinIndex) const;
    
protected:
    std::vector<PinPosition> inputPins;
    std::vector<PinPosition> outputPins;
    
public:
    bool GetInputState(int inputIndex) const;
    bool GetOutputState(int outputIndex) const;
    void SetInputState(int inputIndex, bool state);
    void SetOutputState(int outputIndex, bool state);

    int GetNumInputs() const { return numInputs; }
    int GetNumOutputs() const { return numOutputs; }

    bool IsHovered(Vector2 mousePosition) const;
    int HoveredPin(Vector2 mousePosition) const;

    const std::string& GetTextureKey() const { return textureKey; }
    Texture2D GetTexture() const;

    void SetHighlighted(bool highlight) { isHighlighted = highlight; }
    bool IsHighlighted() const { return isHighlighted; }

    Wire* GetWireAtPin(int pinIndex) const;
    bool CanConnectAtPin(int pinIndex) const;

    void SetComponentManager(ComponentManager* manager) { componentManager = manager; }
    void SetScale(float newScale);
    float GetScale() const { return scale; }

    Vector2 GetScaledSize() const { return {size.x * scale, size.y * scale}; }

    void DrawPins() const;

    // Rotation-related methods
    void Rotate(float angle);
    float GetRotation() const;
    void SetRotation(float newRotation);

    // New methods for origin and drawing
    Vector2 GetOrigin() const;
    Rectangle GetDestRect() const;
    void DrawComponent() const;

public:
    static const int GRID_SIZE = 32; // Size of one grid cell

    static const float PIN_RADIUS;
    static const float PIN_HOVER_RADIUS;
    static bool showDebugFrames;

    static void ToggleDebugFrames() { showDebugFrames = !showDebugFrames; }
    static bool AreDebugFramesEnabled() { return showDebugFrames; }

protected:
    Vector2 position;
    std::string textureKey;
    void DrawDebugFrames() const;
    int numInputs;
    int numOutputs;
    std::vector<bool> inputStates;
    std::vector<bool> outputStates;
    bool isHighlighted;
    Vector2 size = {GRID_SIZE * 2, GRID_SIZE * 2}; // Default size for all components (2x2 grid)
    ComponentManager* componentManager;
    float scale = 1.0f;
    float rotation = 0.0f;
    friend class ConnectionManager;
};

#endif // COMPONENT_H
