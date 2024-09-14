#include "Renderer.h"
#include "../core/Component.h"
#include "../circuit_elements/Wire.h"
#include "../managers/ComponentManager.h"
#include "../core/GameState.h"
#include <raymath.h>

Renderer::Renderer(Camera2D& camera, ComponentManager& componentManager, std::vector<Wire*>& wires)
    : m_camera(camera), m_componentManager(componentManager), m_wires(wires), highlightedWire(nullptr) {
    m_toolbarHeight = static_cast<int>(ORIGINAL_TOOLBAR_HEIGHT * m_globalScaleFactor);
    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();
    m_aspectRatio = static_cast<float>(m_screenWidth) / m_screenHeight;
}

void Renderer::HighlightWireForDeletion(Wire* wire) {
    highlightedWire = wire;
}

void Renderer::HandleResize(int newWidth, int newHeight, Camera2D& camera, int originalWidth, int originalHeight) {
    float newAspectRatio = static_cast<float>(newWidth) / newHeight;
    
    // Calculate new global scale factor
    float scaleX = static_cast<float>(newWidth) / originalWidth;
    float scaleY = static_cast<float>(newHeight) / originalHeight;
    m_globalScaleFactor = std::min(scaleX, scaleY);
    
    // Adjust zoom to maintain view area
    if (newAspectRatio != m_aspectRatio) {
        if (newAspectRatio > m_aspectRatio) {
            camera.zoom *= static_cast<float>(newHeight) / m_screenHeight;
        } else {
            camera.zoom *= static_cast<float>(newWidth) / m_screenWidth;
        }
        camera.zoom = Clamp(camera.zoom, MIN_ZOOM, MAX_ZOOM);
    }
    
    m_componentManager.updateComponentPositions(newWidth, newHeight, originalWidth, originalHeight);
    
    m_screenWidth = newWidth;
    m_screenHeight = newHeight;
    m_aspectRatio = newAspectRatio;
    camera.offset = {static_cast<float>(m_screenWidth)/2.0f, static_cast<float>(m_screenHeight)/2.0f};
    
    m_toolbarHeight = static_cast<int>(ORIGINAL_TOOLBAR_HEIGHT * m_globalScaleFactor);
}

void Renderer::Render(ProgramState currentState, Component* wireStartComponent, int wireStartPin, Vector2 wireEndPos, bool showDebugInfo, Component* selectedComponent, ComponentType currentComponentType, float placementRotation, Vector2 mousePosition, Vector2 worldMousePos) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    BeginMode2D(m_camera);
    
    DrawGrid();
    
    // Draw all placed components
    for (const auto& component : m_componentManager.getComponents()) {
        DrawRotatedComponent(component);
    }
    
    // Draw all wires
    for (const auto& wire : m_wires) {
        if (wire == highlightedWire) {
            // Draw highlighted wire
            wire->Draw(RED);
        } else {
            wire->Draw();
        }
    }

    // Draw wire being placed
    if (currentState == ProgramState::CONNECTING_WIRE && wireStartComponent) {
        Vector2 startPos = wireStartComponent->GetPinPosition(wireStartPin);
        DrawLineEx(startPos, wireEndPos, 2.0f, GRAY);
    }
    
    // Draw hover effects and placement preview
    Vector2 mousePosition = GetMousePosition();
    Vector2 worldMousePos = ScreenToWorld(mousePosition);
    Vector2 snappedPosition = SnapToGrid(worldMousePos);
    
    if (currentState == ProgramState::PLACING_COMPONENT) {
        // Draw placement preview with rotation
        DrawRotatedRectangleLinesEx(Rectangle{snappedPosition.x, snappedPosition.y, 64 * m_globalScaleFactor, 64 * m_globalScaleFactor}, placementRotation, 2, GREEN);
        
        // Draw rotation indicator
        Vector2 rotationIndicator = { snappedPosition.x + 32 * m_globalScaleFactor, snappedPosition.y };
        rotationIndicator = Vector2Rotate(Vector2Subtract(rotationIndicator, snappedPosition), placementRotation * DEG2RAD);
        rotationIndicator = Vector2Add(rotationIndicator, snappedPosition);
        DrawLineEx(snappedPosition, rotationIndicator, 2.0f, RED);
    } else {
        // Draw hover effects for existing components
        for (const auto& component : m_componentManager.getComponents()) {
            if (component->IsHovered(worldMousePos) || component == selectedComponent) {
                Vector2 componentPos = component->GetPosition();
                Vector2 componentSize = component->GetScaledSize();
                DrawRotatedRectangleLinesEx(Rectangle{
                    componentPos.x - componentSize.x / 2, 
                    componentPos.y - componentSize.y / 2, 
                    componentSize.x, 
                    componentSize.y
                }, component->GetRotation(), 2, ORANGE);
        
                // Draw hover effects for pins
                for (int i = 0; i < component->GetNumInputs() + component->GetNumOutputs(); ++i) {
                    Vector2 pinPos = component->GetPinPosition(i);
                    if (Vector2Distance(pinPos, worldMousePos) <= CONNECTION_RADIUS * m_globalScaleFactor) {
                        DrawCircleV(pinPos, CONNECTION_RADIUS * m_globalScaleFactor, Fade(ORANGE, 0.3f));
                        DrawCircleV(pinPos, Component::PIN_RADIUS * m_globalScaleFactor, ORANGE);
                    }
                }
            }
        }
    }
    
    EndMode2D();
    
    DrawToolbar(currentComponentType);
    
    if (showDebugInfo) {
        DrawDebugInfo(currentState, currentComponentType, placementRotation, mousePosition, worldMousePos);
    }
    
    EndDrawing();
}

void Renderer::DrawGrid() {
    Vector2 screenStart = ScreenToWorld({0, static_cast<float>(m_toolbarHeight)});
    Vector2 screenEnd = ScreenToWorld({static_cast<float>(m_screenWidth), static_cast<float>(m_screenHeight)});

    int startX = static_cast<int>(screenStart.x / GRID_SIZE) * GRID_SIZE - GRID_SIZE;
    int startY = static_cast<int>(screenStart.y / GRID_SIZE) * GRID_SIZE - GRID_SIZE;
    int endX = static_cast<int>(screenEnd.x / GRID_SIZE) * GRID_SIZE + GRID_SIZE;
    int endY = static_cast<int>(screenEnd.y / GRID_SIZE) * GRID_SIZE + GRID_SIZE;

    for (int i = startX; i <= endX; i += GRID_SIZE) {
        DrawLineV({static_cast<float>(i), screenStart.y}, {static_cast<float>(i), screenEnd.y}, LIGHTGRAY);
    }
    for (int i = startY; i <= endY; i += GRID_SIZE) {
        DrawLineV({screenStart.x, static_cast<float>(i)}, {screenEnd.x, static_cast<float>(i)}, LIGHTGRAY);
    }
}

void Renderer::DrawToolbar(ComponentType currentComponentType) {
    int buttonWidth = m_screenWidth / 4;
    int fontSize = static_cast<int>(20 * m_globalScaleFactor);
    
    DrawRectangle(0, 0, m_screenWidth, m_toolbarHeight, LIGHTGRAY);
    
    const char* buttonTexts[] = {"AND (A)", "OR (O)", "NOT (N)", "INPUT (I)"};
    
    for (int i = 0; i < 4; i++) {
        DrawRectangleLines(buttonWidth * i, 0, buttonWidth, m_toolbarHeight, BLACK);
        int textWidth = MeasureText(buttonTexts[i], fontSize);
        int textX = buttonWidth * i + (buttonWidth - textWidth) / 2;
        int textY = (m_toolbarHeight - fontSize) / 2;
        DrawText(buttonTexts[i], textX, textY, fontSize, BLACK);
    }
    
    // Highlight the current selected component
    int selectedButton = static_cast<int>(currentComponentType);
    DrawRectangle(buttonWidth * selectedButton, 0, buttonWidth, m_toolbarHeight, Fade(YELLOW, 0.5f));
}

void Renderer::DrawDebugInfo(ProgramState currentState, ComponentType currentComponentType, float placementRotation, Vector2 mousePosition, Vector2 worldMousePos) {
    int screenWidth = GetScreenWidth();
    int rightAlignX = screenWidth - 300; // Adjust this value to change the distance from the right edge

    DrawText(TextFormat("FPS: %d", GetFPS()), rightAlignX, m_toolbarHeight + 10, 20, DARKGRAY);
    DrawText(TextFormat("Components: %zd", m_componentManager.getComponents().size()), rightAlignX, m_toolbarHeight + 40, 20, DARKGRAY);
    DrawText(TextFormat("Wires: %zd", m_wires.size()), rightAlignX, m_toolbarHeight + 70, 20, DARKGRAY);
    DrawText(TextFormat("State: %s", 
        currentState == ProgramState::IDLE ? "IDLE" :
        currentState == ProgramState::PLACING_COMPONENT ? "PLACING" :
        currentState == ProgramState::CONNECTING_WIRE ? "CONNECTING" :
        currentState == ProgramState::PANNING ? "PANNING" : "SELECTING"), rightAlignX, m_toolbarHeight + 100, 20, DARKGRAY);
    DrawText(TextFormat("Current Component: %s", 
        currentComponentType == ComponentType::AND ? "AND" : 
        currentComponentType == ComponentType::OR ? "OR" : 
        currentComponentType == ComponentType::NOT ? "NOT" : "INPUT"), rightAlignX, m_toolbarHeight + 130, 20, DARKGRAY);
    DrawText(TextFormat("Camera Zoom: %.2f", m_camera.zoom), rightAlignX, m_toolbarHeight + 160, 20, DARKGRAY);
    DrawText(TextFormat("Camera Target: (%.2f, %.2f)", m_camera.target.x, m_camera.target.y), rightAlignX, m_toolbarHeight + 190, 20, DARKGRAY);
    DrawText(TextFormat("Placement Rotation: %.2f", placementRotation), rightAlignX, m_toolbarHeight + 220, 20, DARKGRAY);
    DrawText(TextFormat("Debug Frames: %s", Component::AreDebugFramesEnabled() ? "ON" : "OFF"), rightAlignX, m_toolbarHeight + 250, 20, DARKGRAY);
    DrawText(TextFormat("Mouse Position: (%.2f, %.2f)", mousePosition.x, mousePosition.y), rightAlignX, m_toolbarHeight + 280, 20, DARKGRAY);
    DrawText(TextFormat("World Mouse Position: (%.2f, %.2f)", worldMousePos.x, worldMousePos.y), rightAlignX, m_toolbarHeight + 310, 20, DARKGRAY);
}

Vector2 Renderer::ScreenToWorld(Vector2 screenPos) {
    return GetScreenToWorld2D(screenPos, m_camera);
}

Vector2 Renderer::WorldToScreen(Vector2 worldPos) {
    return GetWorldToScreen2D(worldPos, m_camera);
}

Vector2 Renderer::SnapToGrid(Vector2 position) {
    return {
        float(int(position.x / GRID_SIZE) * GRID_SIZE),
        float(int(position.y / GRID_SIZE) * GRID_SIZE)
    };
}

int Renderer::GetToolbarHeight() const {
    return m_toolbarHeight;
}

void Renderer::DrawRotatedComponent(const Component* component) {
    component->Draw();
}

void Renderer::DrawRotatedRectangleLinesEx(Rectangle rec, float rotation, float lineThick, Color color) {
    Vector2 center = { rec.x + rec.width / 2, rec.y + rec.height / 2 };
    Vector2 corners[4] = {
        { -rec.width / 2, -rec.height / 2 },
        { rec.width / 2, -rec.height / 2 },
        { rec.width / 2, rec.height / 2 },
        { -rec.width / 2, rec.height / 2 }
    };
    
    for (int i = 0; i < 4; i++) {
        corners[i] = Vector2Rotate(corners[i], rotation * DEG2RAD);
        corners[i] = Vector2Add(corners[i], center);
    }
    
    for (int i = 0; i < 4; i++) {
        DrawLineEx(corners[i], corners[(i + 1) % 4], lineThick, color);
    }
}
