#include "Wire.h"
#include "../managers/ConnectionManager.h"
#include "../core/Component.h"
#include <cmath>

Wire::Wire(Component* start, int startPin, Component* end, int endPin)
    : startComponent(start), endComponent(end),
      startPinIndex(startPin), endPinIndex(endPin),
      signalState(false)
{
    CalculateWirePoints();
}

Wire::~Wire()
{
    // The ConnectionManager is responsible for removing the wire from its list
}

void Wire::Update()
{
    // Update the wire's signal state based on the start component's output
    signalState = startComponent->GetOutputState(startPinIndex);
    
    // Propagate the signal to the end component
    endComponent->SetInputState(endPinIndex, signalState);

    // Recalculate wire points in case components have moved
    CalculateWirePoints();
}

void Wire::Draw() const
{
    Color wireColor = signalState ? RED : GRAY;
    
    for (size_t i = 1; i < points.size(); ++i)
    {
        DrawLineEx(points[i-1], points[i], 2.0f, wireColor);
    }
}

void Wire::CalculateWirePoints()
{
    Vector2 startPos = startComponent->GetPinPosition(startPinIndex);
    Vector2 endPos = endComponent->GetPinPosition(endPinIndex);

    points.clear();
    points.push_back(startPos);

    // Calculate midpoint
    Vector2 midPoint = {
        (startPos.x + endPos.x) / 2,
        (startPos.y + endPos.y) / 2
    };

    // Add intermediate points for a more natural wire path
    points.push_back({midPoint.x, startPos.y});
    points.push_back({midPoint.x, endPos.y});

    points.push_back(endPos);
}

void Wire::UpdateEndPosition(Vector2 newEndPos)
{
    // This method is now only used for temporary wires during dragging
    points.back() = newEndPos;
    CalculateWirePoints();
}