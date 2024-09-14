#include "Renderer.h"
#include "../core/Component.h"
#include "../circuit_elements/Wire.h"
#include "../managers/ComponentManager.h"
#include "../core/GameState.h"
#include "../core/Grid.h"
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
    Vector2 snappedPosition = SnapToGrid(worldMousePos);
    
    if (currentState == ProgramState::PLACING_COMPONENT) {
        // Draw placement preview with rotation
        Vector2 componentSize = {Grid::GRID_SIZE * 2 * m_globalScaleFactor, Grid::GRID_SIZE * 2 * m_globalScaleFactor};
        Rectangle previewRect = {
            snappedPosition.x - componentSize.x / 2,
            snappedPosition.y - componentSize.y / 2,
            componentSize.x,
            componentSize.y
        };
        DrawRotatedRectangleLinesEx(previewRect, placementRotation, 2, GREEN);
        
        // Draw rotation indicator
        Vector2 rotationIndicator = { snappedPosition.x + componentSize.x / 2, snappedPosition.y };
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
    Grid::Draw(screenStart, screenEnd);
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
    int screenHeight = GetScreenHeight();
    int rightAlignX = screenWidth - 350; // Increased to accommodate longer text
    int fontSize = 16; // Reduced font size to fit more information
    int lineHeight = fontSize + 4; // Spacing between lines

    // Left side debug info
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, m_toolbarHeight + 10, fontSize, DARKGRAY);
    DrawText(TextFormat("Components: %zd", m_componentManager.getComponents().size()), 10, m_toolbarHeight + 10 + lineHeight, fontSize, DARKGRAY);
    DrawText(TextFormat("Wires: %zd", m_wires.size()), 10, m_toolbarHeight + 10 + 2 * lineHeight, fontSize, DARKGRAY);
    DrawText(TextFormat("State: %s", 
        currentState == ProgramState::IDLE ? "IDLE" :
        currentState == ProgramState::PLACING_COMPONENT ? "PLACING" :
        currentState == ProgramState::CONNECTING_WIRE ? "CONNECTING" :
        currentState == ProgramState::PANNING ? "PANNING" : 
        currentState == ProgramState::SELECTING ? "SELECTING" :
        "MOVING_COMPONENT"), 10, m_toolbarHeight + 10 + 3 * lineHeight, fontSize, DARKGRAY);
    DrawText(TextFormat("Current Component: %s", 
        currentComponentType == ComponentType::AND ? "AND" : 
        currentComponentType == ComponentType::OR ? "OR" : 
        currentComponentType == ComponentType::NOT ? "NOT" : "INPUT"), 10, m_toolbarHeight + 10 + 4 * lineHeight, fontSize, DARKGRAY);
    DrawText(TextFormat("Camera Zoom: %.2f", m_camera.zoom), 10, m_toolbarHeight + 10 + 5 * lineHeight, fontSize, DARKGRAY);
    DrawText(TextFormat("Camera Target: (%.2f, %.2f)", m_camera.target.x, m_camera.target.y), 10, m_toolbarHeight + 10 + 6 * lineHeight, fontSize, DARKGRAY);
    DrawText(TextFormat("Placement Rotation: %.2f", placementRotation), 10, m_toolbarHeight + 10 + 7 * lineHeight, fontSize, DARKGRAY);
    DrawText(TextFormat("Debug Frames: %s", Component::AreDebugFramesEnabled() ? "ON" : "OFF"), 10, m_toolbarHeight + 10 + 8 * lineHeight, fontSize, DARKGRAY);

    // Right side debug info
    DrawText(TextFormat("Screen Mouse: (%.1f, %.1f)", mousePosition.x, mousePosition.y), rightAlignX, m_toolbarHeight + 10, fontSize, DARKGRAY);
    DrawText(TextFormat("World Mouse: (%.1f, %.1f)", worldMousePos.x, worldMousePos.y), rightAlignX, m_toolbarHeight + 10 + lineHeight, fontSize, DARKGRAY);
    DrawText(TextFormat("Left Mouse: %s", IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? "DOWN" : "UP"), rightAlignX, m_toolbarHeight + 10 + 2 * lineHeight, fontSize, DARKGRAY);
}

Vector2 Renderer::ScreenToWorld(Vector2 screenPos) {
    return GetScreenToWorld2D(screenPos, m_camera);
}

Vector2 Renderer::WorldToScreen(Vector2 worldPos) {
    return GetWorldToScreen2D(worldPos, m_camera);
}

Vector2 Renderer::SnapToGrid(Vector2 position) {
    return {
        floorf(position.x / Grid::GRID_SIZE) * Grid::GRID_SIZE,
        floorf(position.y / Grid::GRID_SIZE) * Grid::GRID_SIZE
    };
}

int Renderer::GetToolbarHeight() const {
    return m_toolbarHeight;
}

void Renderer::DrawRotatedComponent(const Component* component) {
    Texture2D texture = component->GetTexture();
    Vector2 position = component->GetPosition();
    float rotation = component->GetRotation();
    Vector2 size = component->GetScaledSize();
    
    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
    Rectangle dest = { position.x, position.y, size.x * m_camera.zoom, size.y * m_camera.zoom };
    Vector2 origin = { size.x * m_camera.zoom / 2, size.y * m_camera.zoom / 2 };
    
    DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
    component->DrawPins();
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
