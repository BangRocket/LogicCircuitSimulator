#ifndef WIRE_H
#define WIRE_H

#include "raylib.h"
#include <vector>

class Component;

class Wire {
public:
    Wire(Component* startComponent, int startPinIndex, Component* endComponent, int endPinIndex);
    void Update();
    void Draw() const;
    
    Component* GetStartComponent() const { return startComponent; }
    Component* GetEndComponent() const { return endComponent; }
    int GetStartPinIndex() const { return startPinIndex; }
    int GetEndPinIndex() const { return endPinIndex; }

private:
    Component* startComponent;
    Component* endComponent;
    int startPinIndex;
    int endPinIndex;
    std::vector<Vector2> points;
    bool signalState;

    void CalculateWirePoints();
};

#endif // WIRE_H