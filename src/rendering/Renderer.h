#pragma once

#include "raylib.h"
#include "../managers/ComponentManager.h"
#include "../core/GameState.h"
#include <vector>

class Wire;
class Component;

class Renderer {
public:
    Renderer(Camera2D& camera, ComponentManager& componentManager, std::vector<Wire*>& wires);

    void HandleResize(int newWidth, int newHeight, Camera2D& camera, int originalWidth, int originalHeight);
    void Render(ProgramState currentState, Component* wireStartComponent, int wireStartPin, Vector2 wireEndPos, bool showDebugInfo, Component* selectedComponent, ComponentType currentComponentType, float placementRotation, Vector2 mousePosition, Vector2 worldMousePos);
    
    Vector2 ScreenToWorld(Vector2 screenPos);
    Vector2 WorldToScreen(Vector2 worldPos);
    Vector2 SnapToGrid(Vector2 position);
    int GetToolbarHeight() const;

    void HighlightWireForDeletion(Wire* wire);

private:
    void DrawGrid();
    void DrawToolbar(ComponentType currentComponentType);
    void DrawDebugInfo(ProgramState currentState, ComponentType currentComponentType, float placementRotation, Vector2 mousePosition, Vector2 worldMousePos);
    void DrawRotatedComponent(const Component* component);
    void DrawRotatedRectangleLinesEx(Rectangle rec, float rotation, float lineThick, Color color);

    Wire* highlightedWire;

    Camera2D& m_camera;
    ComponentManager& m_componentManager;
    std::vector<Wire*>& m_wires;

    int m_screenWidth = 800;
    int m_screenHeight = 600;
    float m_aspectRatio = 4.0f / 3.0f;
    float m_globalScaleFactor = 1.0f;
    int m_toolbarHeight = 40;

    static constexpr int ORIGINAL_SCREEN_WIDTH = 800;
    static constexpr int ORIGINAL_SCREEN_HEIGHT = 600;
    static constexpr int ORIGINAL_TOOLBAR_HEIGHT = 40;
    static constexpr int GRID_SIZE = 32;
    static constexpr float MIN_ZOOM = 0.5f;
    static constexpr float MAX_ZOOM = 2.0f;
    static constexpr float PIN_RADIUS = 5.0f;
    static constexpr float CONNECTION_RADIUS = 20.0f;
};
