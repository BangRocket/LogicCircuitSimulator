#include "Wire.h"
#include "../managers/ConnectionManager.h"
#include "../core/Component.h"
#include <cmath>
#include <raymath.h>

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

    // Recalculate wire points in case components have moved or rotated
    RecalculateWirePath();
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

bool Wire::IsPointOnWire(Vector2 point) const {
    for (size_t i = 1; i < points.size(); ++i) {
        if (IsPointOnLineSegment(point, points[i-1], points[i])) {
            return true;
        }
    }
    return false;
}

bool Wire::IsPointOnLineSegment(Vector2 point, Vector2 lineStart, Vector2 lineEnd) const {
    const float TOLERANCE = 5.0f;  // Adjust this value to change the "thickness" of the wire for selection
    
    float d1 = Vector2Distance(point, lineStart);
    float d2 = Vector2Distance(point, lineEnd);
    float lineLen = Vector2Distance(lineStart, lineEnd);
    
    if (d1 + d2 >= lineLen - TOLERANCE && d1 + d2 <= lineLen + TOLERANCE) {
        return true;
    }
    return false;
}

void Wire::UpdateEndPosition(Vector2 newEndPos)
{
    // This method is now only used for temporary wires during dragging
    points.back() = newEndPos;
    CalculateWirePoints();
}

void Wire::UpdateConnectionsAfterRotation()
{
    // Update pin positions after rotation
    Vector2 newStartPos = startComponent->GetPinPosition(startPinIndex);
    Vector2 newEndPos = endComponent->GetPinPosition(endPinIndex);

    // Update the first and last points of the wire
    if (!points.empty()) {
        points.front() = newStartPos;
        points.back() = newEndPos;
    }

    // Recalculate the wire path
    RecalculateWirePath();
}

void Wire::RecalculateWirePath()
{
    Vector2 startPos = startComponent->GetPinPosition(startPinIndex);
    Vector2 endPos = endComponent->GetPinPosition(endPinIndex);

    // Keep only the start and end points
    points.clear();
    points.push_back(startPos);

    // Calculate new midpoint
    Vector2 midPoint = {
        (startPos.x + endPos.x) / 2,
        (startPos.y + endPos.y) / 2
    };

    // Determine the best path based on component orientations
    float startAngle = startComponent->GetRotation();
    float endAngle = endComponent->GetRotation();

    // Simplified path calculation (can be improved for more complex routing)
    if (std::abs(startPos.x - endPos.x) > std::abs(startPos.y - endPos.y)) {
        // Horizontal-dominant path
        points.push_back({midPoint.x, startPos.y});
        points.push_back({midPoint.x, endPos.y});
    } else {
        // Vertical-dominant path
        points.push_back({startPos.x, midPoint.y});
        points.push_back({endPos.x, midPoint.y});
    }

    points.push_back(endPos);
}
