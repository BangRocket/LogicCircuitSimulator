#ifndef WIRE_H
#define WIRE_H

#include "raylib.h"
#include <vector>

class Component;
class ConnectionManager;

class Wire {
public:
    Wire(Component* startComponent, int startPinIndex, Component* endComponent, int endPinIndex);
    ~Wire();

    void Update();
    void Draw() const;
    
    Component* GetStartComponent() const { return startComponent; }
    Component* GetEndComponent() const { return endComponent; }
    int GetStartPinIndex() const { return startPinIndex; }
    int GetEndPinIndex() const { return endPinIndex; }

    void UpdateEndPosition(Vector2 newEndPos);
    void UpdateConnectionsAfterRotation();
    void RecalculateWirePath();

private:
    Component* startComponent;
    Component* endComponent;
    int startPinIndex;
    int endPinIndex;
    std::vector<Vector2> points;
    bool signalState;

    void CalculateWirePoints();

    friend class ConnectionManager;
};

#endif // WIRE_H