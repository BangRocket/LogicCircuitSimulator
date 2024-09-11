#include "raylib.h"
#include "core/Component.h"
#include "gates/AndGate.h"
#include "gates/OrGate.h"
#include "gates/NotGate.h"
#include "circuit_elements/InputSwitch.h"
#include "circuit_elements/Wire.h"
#include "managers/ResourceManager.h"
#include <iostream>
#include <vector>
#include <xutility>
#include <raymath.h>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 32;
const float MIN_ZOOM = 0.5f;
const float MAX_ZOOM = 2.0f;
const int TOOLBAR_HEIGHT = 40;
const float PIN_RADIUS = 5.0f;

// Enumeration for component types
enum class ComponentType {
    AND,
    OR,
    NOT,
    INPUT_SWITCH
};

// Enumeration for program states
enum class ProgramState {
    PLACING_COMPONENT,
    CONNECTING_WIRE,
    IDLE,
    PANNING
};

// Function prototypes
void HandleInput();
void Update();
void Render();
void DrawGrid();
void DrawToolbar();
Vector2 SnapToGrid(Vector2 position);
void DrawDebugInfo();
Component* GetComponentAtPosition(Vector2 position);
int GetPinAtPosition(Component* component, Vector2 position);
Vector2 ScreenToWorld(Vector2 screenPos);
Vector2 WorldToScreen(Vector2 worldPos);

// Global variables
ProgramState currentState = ProgramState::IDLE;
ComponentType currentComponentType = ComponentType::AND;
std::vector<Component*> components;
std::vector<Wire*> wires;
Component* wireStartComponent = nullptr;
int wireStartPin = -1;
Vector2 wireEndPos = {0, 0};
bool showDebugInfo = true;

// Camera variables
Camera2D camera = { 0 };
Vector2 prevMousePos = { 0, 0 };

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Logic Circuit Simulator");
    SetTargetFPS(60);

    // Initialize camera
    camera.target = {0, 0};
    camera.offset = {SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // Load textures
    ResourceManager::getInstance().loadSVGTexture("and_gate", "assets/and_gate.svg", 64, 64);
    ResourceManager::getInstance().loadSVGTexture("or_gate", "assets/or_gate.svg", 64, 64);
    ResourceManager::getInstance().loadSVGTexture("not_gate", "assets/not_gate.svg", 64, 64);
    ResourceManager::getInstance().loadSVGTexture("input_switch", "assets/input_switch.svg", 64, 64);

    while (!WindowShouldClose()) {
        HandleInput();
        Update();
        Render();
    }

    // Clean up components and wires
    for (auto& component : components) {
        delete component;
    }
    components.clear();

    for (auto& wire : wires) {
        delete wire;
    }
    wires.clear();

    // Unload textures
    ResourceManager::getInstance().unloadAllTextures();

    CloseWindow();
    return 0;
}

void HandleInput() {
    Vector2 mousePosition = GetMousePosition();
    Vector2 worldMousePos = ScreenToWorld(mousePosition);
    Vector2 snappedPosition = SnapToGrid(worldMousePos);

    // Handle toolbar interactions
    if (mousePosition.y < TOOLBAR_HEIGHT) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int buttonWidth = SCREEN_WIDTH / 4;
            int clickedButton = mousePosition.x / buttonWidth;
            switch (clickedButton) {
                case 0: currentComponentType = ComponentType::AND; break;
                case 1: currentComponentType = ComponentType::OR; break;
                case 2: currentComponentType = ComponentType::NOT; break;
                case 3: currentComponentType = ComponentType::INPUT_SWITCH; break;
            }
        }
        return;  // Exit early if interacting with toolbar
    }

    // Handle panning
    if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
        Vector2 mouseDelta = Vector2Subtract(mousePosition, prevMousePos);
        camera.target = Vector2Subtract(camera.target, Vector2Scale(mouseDelta, 1.0f/camera.zoom));
        currentState = ProgramState::PANNING;
    } else if (currentState == ProgramState::PANNING) {
        currentState = ProgramState::IDLE;
    }

    // Handle zooming
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector2 mouseWorldPos = ScreenToWorld(mousePosition);
        camera.offset = mousePosition;
        camera.target = mouseWorldPos;
        
        camera.zoom += wheel * 0.05f;
        camera.zoom = Clamp(camera.zoom, MIN_ZOOM, MAX_ZOOM);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (currentState) {
            case ProgramState::IDLE:
                {
                    Component* clickedComponent = GetComponentAtPosition(worldMousePos);
                    if (clickedComponent) {
                        int pinIndex = GetPinAtPosition(clickedComponent, worldMousePos);
                        if (pinIndex != -1) {
                            wireStartComponent = clickedComponent;
                            wireStartPin = pinIndex;
                            currentState = ProgramState::CONNECTING_WIRE;
                        } else {
                            // Toggle input switch if clicked
                            InputSwitch* inputSwitch = dynamic_cast<InputSwitch*>(clickedComponent);
                            if (inputSwitch) {
                                inputSwitch->ToggleState();
                            }
                        }
                    } else {
                        currentState = ProgramState::PLACING_COMPONENT;
                    }
                }
                break;
            case ProgramState::PLACING_COMPONENT:
                {
                    Component* newComponent = nullptr;
                    switch (currentComponentType) {
                        case ComponentType::AND:
                            newComponent = new AndGate(snappedPosition);
                            std::cout << "New AND gate created at: (" << snappedPosition.x << ", " << snappedPosition.y << ")" << std::endl;
                            break;
                        case ComponentType::OR:
                            newComponent = new OrGate(snappedPosition);
                            std::cout << "New OR gate created at: (" << snappedPosition.x << ", " << snappedPosition.y << ")" << std::endl;
                            break;
                        case ComponentType::NOT:
                            newComponent = new NotGate(snappedPosition);
                            std::cout << "New NOT gate created at: (" << snappedPosition.x << ", " << snappedPosition.y << ")" << std::endl;
                            break;
                        case ComponentType::INPUT_SWITCH:
                            newComponent = new InputSwitch(snappedPosition);
                            std::cout << "New InputSwitch created at: (" << snappedPosition.x << ", " << snappedPosition.y << ")" << std::endl;
                            break;
                    }
                    if (newComponent) {
                        components.push_back(newComponent);
                    }
                    currentState = ProgramState::IDLE;
                }
                break;
            case ProgramState::CONNECTING_WIRE:
                {
                    Component* endComponent = GetComponentAtPosition(worldMousePos);
                    if (endComponent && endComponent != wireStartComponent) {
                        int endPin = GetPinAtPosition(endComponent, worldMousePos);
                        if (endPin != -1) {
                            Wire* newWire = new Wire(wireStartComponent, wireStartPin, endComponent, endPin);
                            wires.push_back(newWire);
                        }
                    }
                    wireStartComponent = nullptr;
                    wireStartPin = -1;
                    currentState = ProgramState::IDLE;
                }
                break;
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        // Cancel wire placement or component placement
        wireStartComponent = nullptr;
        wireStartPin = -1;
        currentState = ProgramState::IDLE;
    }

    if (IsKeyPressed(KEY_D)) {
        showDebugInfo = !showDebugInfo;
    }

    wireEndPos = worldMousePos;
    prevMousePos = mousePosition;
}

void Update() {
    // Update all wires
    for (auto& wire : wires) {
        wire->Update();
    }

    // Update all components
    for (auto& component : components) {
        component->Update();
    }
}

void Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    BeginMode2D(camera);
    
    DrawGrid();
    
    // Draw all placed components
    for (const auto& component : components) {
        component->Draw();
    }
    
    // Draw all wires
    for (const auto& wire : wires) {
        wire->Draw();
    }

    // Draw wire being placed
    if (currentState == ProgramState::CONNECTING_WIRE && wireStartComponent) {
        Vector2 startPos = wireStartComponent->GetPinPosition(wireStartPin);
        DrawLineEx(startPos, wireEndPos, 2.0f, GRAY);
    }
    
    // Draw hover effects
    Vector2 mousePosition = GetMousePosition();
    Vector2 worldMousePos = ScreenToWorld(mousePosition);
    for (const auto& component : components) {
        if (component->IsHovered(worldMousePos)) {
            Vector2 screenPos = WorldToScreen(component->GetPosition());
            DrawRectangleLinesEx({screenPos.x, screenPos.y, 64, 64}, 2, YELLOW);
            
            int hoveredPin = component->HoveredPin(worldMousePos);
            if (hoveredPin != -1) {
                Vector2 pinPos = WorldToScreen(component->GetPinPosition(hoveredPin));
                DrawCircleV(pinPos, PIN_RADIUS + 2, YELLOW);
            }
        }
    }
    
    EndMode2D();
    
    DrawToolbar();
    
    if (showDebugInfo) {
        DrawDebugInfo();
    }
    
    EndDrawing();
}

void DrawToolbar() {
    DrawRectangle(0, 0, SCREEN_WIDTH, TOOLBAR_HEIGHT, LIGHTGRAY);
    
    int buttonWidth = SCREEN_WIDTH / 4;
    
    DrawRectangleLines(0, 0, buttonWidth, TOOLBAR_HEIGHT, BLACK);
    DrawText("AND", 10, 10, 20, BLACK);
    
    DrawRectangleLines(buttonWidth, 0, buttonWidth, TOOLBAR_HEIGHT, BLACK);
    DrawText("OR", buttonWidth + 10, 10, 20, BLACK);
    
    DrawRectangleLines(buttonWidth * 2, 0, buttonWidth, TOOLBAR_HEIGHT, BLACK);
    DrawText("NOT", buttonWidth * 2 + 10, 10, 20, BLACK);
    
    DrawRectangleLines(buttonWidth * 3, 0, buttonWidth, TOOLBAR_HEIGHT, BLACK);
    DrawText("INPUT", buttonWidth * 3 + 10, 10, 20, BLACK);
    
    // Highlight the current selected component
    int selectedButton = static_cast<int>(currentComponentType);
    DrawRectangle(buttonWidth * selectedButton, 0, buttonWidth, TOOLBAR_HEIGHT, ColorAlpha(YELLOW, 0.5f));
}

void DrawGrid() {
    Vector2 screenStart = ScreenToWorld({0, static_cast<float>(TOOLBAR_HEIGHT)});
    Vector2 screenEnd = ScreenToWorld({static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)});

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

Vector2 SnapToGrid(Vector2 position) {
    return {
        float(int(position.x / GRID_SIZE) * GRID_SIZE),
        float(int(position.y / GRID_SIZE) * GRID_SIZE)
    };
}

void DrawDebugInfo() {
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, TOOLBAR_HEIGHT + 10, 20, DARKGRAY);
    DrawText(TextFormat("Components: %zd", std::ssize(components)), 10, TOOLBAR_HEIGHT + 40, 20, DARKGRAY);
    DrawText(TextFormat("Wires: %zd", std::ssize(wires)), 10, TOOLBAR_HEIGHT + 70, 20, DARKGRAY);
    DrawText(TextFormat("State: %s", 
        currentState == ProgramState::IDLE ? "IDLE" :
        currentState == ProgramState::PLACING_COMPONENT ? "PLACING" :
        currentState == ProgramState::CONNECTING_WIRE ? "CONNECTING" : "PANNING"), 10, TOOLBAR_HEIGHT + 100, 20, DARKGRAY);
    DrawText(TextFormat("Current Component: %s", 
        currentComponentType == ComponentType::AND ? "AND" : 
        currentComponentType == ComponentType::OR ? "OR" : 
        currentComponentType == ComponentType::NOT ? "NOT" : "INPUT"), 10, TOOLBAR_HEIGHT + 130, 20, DARKGRAY);
    DrawText(TextFormat("Camera Zoom: %.2f", camera.zoom), 10, TOOLBAR_HEIGHT + 160, 20, DARKGRAY);
    DrawText(TextFormat("Camera Target: (%.2f, %.2f)", camera.target.x, camera.target.y), 10, TOOLBAR_HEIGHT + 190, 20, DARKGRAY);
}

Component* GetComponentAtPosition(Vector2 position) {
    for (auto& component : components) {
        if (component->IsHovered(position)) {
            return component;
        }
    }
    return nullptr;
}

int GetPinAtPosition(Component* component, Vector2 position) {
    if (!component) return -1;
    return component->HoveredPin(position);
}

Vector2 ScreenToWorld(Vector2 screenPos) {
    return Vector2Add(Vector2Scale(Vector2Subtract(screenPos, camera.offset), 1/camera.zoom), camera.target);
}

Vector2 WorldToScreen(Vector2 worldPos) {
    return Vector2Add(Vector2Scale(Vector2Subtract(worldPos, camera.target), camera.zoom), camera.offset);
}
