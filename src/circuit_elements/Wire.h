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
    void Draw(Color color = GRAY) const;
    
    Component* GetStartComponent() const { return startComponent; }
    Component* GetEndComponent() const { return endComponent; }
    int GetStartPinIndex() const { return startPinIndex; }
    int GetEndPinIndex() const { return endPinIndex; }

    void UpdateEndPosition(Vector2 newEndPos);
    void UpdateConnectionsAfterRotation();
    void RecalculateWirePath();
    bool IsPointOnWire(Vector2 point) const;

private:
    Component* startComponent;
    Component* endComponent;
    int startPinIndex;
    int endPinIndex;
    std::vector<Vector2> points;
    bool signalState;

    void CalculateWirePoints();
    bool IsPointOnLineSegment(Vector2 point, Vector2 lineStart, Vector2 lineEnd) const;

    friend class ConnectionManager;
};

#endif // WIRE_H
