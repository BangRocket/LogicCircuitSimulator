#include "raylib.h"
#include "core/Component.h"
#include "gates/AndGate.h"
#include "gates/OrGate.h"
#include "gates/NotGate.h"
#include "circuit_elements/InputSwitch.h"
#include "circuit_elements/Wire.h"
#include "managers/ResourceManager.h"
#include "managers/ComponentManager.h"
#include "managers/ConnectionManager.h"
#include "rendering/Renderer.h"
#include "core/GameState.h"
#include <iostream>
#include <vector>
#include <raymath.h>

// Constants
const int ORIGINAL_SCREEN_WIDTH = 800;
const int ORIGINAL_SCREEN_HEIGHT = 600;
const int GRID_SIZE = 32;
const float MIN_ZOOM = 0.5f;
const float MAX_ZOOM = 2.0f;
const float CONNECTION_RADIUS = 20.0f;
const float ROTATION_STEP = 90.0f;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// Function prototypes
void HandleInput();
void Update();
Component* GetComponentAtPosition(Vector2 position);
int GetPinAtPosition(Component* component, Vector2 position);
void UpdateWiresForComponent(Component* component);
Wire* GetWireAtPosition(Vector2 position);

// Global variables
ProgramState currentState = ProgramState::IDLE;
ComponentType currentComponentType = ComponentType::AND;
std::vector<Wire*> wires;
Component* wireStartComponent = nullptr;
int wireStartPin = -1;
Vector2 wireEndPos = {0, 0};
bool showDebugInfo = true;
Component* selectedComponent = nullptr;
float placementRotation = 0.0f;

// Camera variables
Camera2D camera = { 0 };
Vector2 prevMousePos = { 0, 0 };

// Renderer instance
Renderer* renderer = nullptr;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Logic Circuit Simulator");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
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

    ComponentManager::getInstance().setInitialScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create Renderer instance
    renderer = new Renderer(camera, ComponentManager::getInstance(), wires);

    while (!WindowShouldClose()) {
        HandleInput();
        Update();
        renderer->Render(currentState, wireStartComponent, wireStartPin, wireEndPos, showDebugInfo, selectedComponent, currentComponentType, placementRotation);
    }

    // Clean up
    delete renderer;

    // Clean up components and wires
    for (auto& component : ComponentManager::getInstance().getComponents()) {
        delete component;
    }
    ComponentManager::getInstance().getComponents().clear();

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
    Vector2 worldMousePos = renderer->ScreenToWorld(mousePosition);
    Vector2 snappedPosition = renderer->SnapToGrid(worldMousePos);

    // Handle toolbar interactions
    if (mousePosition.y < renderer->GetToolbarHeight()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int buttonWidth = SCREEN_WIDTH / 4;
            int clickedButton = mousePosition.x / buttonWidth;
            switch (clickedButton) {
                case 0: currentComponentType = ComponentType::AND; currentState = ProgramState::PLACING_COMPONENT; break;
                case 1: currentComponentType = ComponentType::OR; currentState = ProgramState::PLACING_COMPONENT; break;
                case 2: currentComponentType = ComponentType::NOT; currentState = ProgramState::PLACING_COMPONENT; break;
                case 3: currentComponentType = ComponentType::INPUT_SWITCH; currentState = ProgramState::PLACING_COMPONENT; break;
            }
        }
        return;  // Exit early if interacting with toolbar
    }

    // Handle keyboard shortcuts for placing components
    if (IsKeyPressed(KEY_A)) { currentComponentType = ComponentType::AND; currentState = ProgramState::PLACING_COMPONENT; }
    if (IsKeyPressed(KEY_O)) { currentComponentType = ComponentType::OR; currentState = ProgramState::PLACING_COMPONENT; }
    if (IsKeyPressed(KEY_N)) { currentComponentType = ComponentType::NOT; currentState = ProgramState::PLACING_COMPONENT; }
    if (IsKeyPressed(KEY_I)) { currentComponentType = ComponentType::INPUT_SWITCH; currentState = ProgramState::PLACING_COMPONENT; }

    // Handle rotation
    if (IsKeyPressed(KEY_R)) {
        if (currentState == ProgramState::PLACING_COMPONENT) {
            placementRotation += ROTATION_STEP;
            if (placementRotation >= 360.0f) placementRotation -= 360.0f;
        } else if (selectedComponent) {
            selectedComponent->Rotate(ROTATION_STEP);
            UpdateWiresForComponent(selectedComponent);
        }
    }

    // Handle panning with right mouse button
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 mouseDelta = Vector2Subtract(mousePosition, prevMousePos);
        camera.target = Vector2Subtract(camera.target, Vector2Scale(mouseDelta, 1.0f/camera.zoom));
        currentState = ProgramState::PANNING;
    } else if (currentState == ProgramState::PANNING) {
        currentState = ProgramState::IDLE;
    }

    // Handle zooming
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector2 mouseWorldPos = renderer->ScreenToWorld(mousePosition);
        camera.offset = mousePosition;
        camera.target = mouseWorldPos;
        
        float prevZoom = camera.zoom;
        camera.zoom += wheel * 0.05f;
        camera.zoom = Clamp(camera.zoom, MIN_ZOOM, MAX_ZOOM);
        
        // Update component scales
        float scaleChange = camera.zoom / prevZoom;
        for (auto& component : ComponentManager::getInstance().getComponents()) {
            component->SetScale(component->GetScale() * scaleChange);
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (currentState) {
            case ProgramState::IDLE:
            case ProgramState::SELECTING:
                {
                    Component* clickedComponent = GetComponentAtPosition(worldMousePos);
                    if (clickedComponent) {
                        selectedComponent = clickedComponent;
                        currentState = ProgramState::SELECTING;
                        // Toggle input switch if clicked
                        InputSwitch* inputSwitch = dynamic_cast<InputSwitch*>(clickedComponent);
                        if (inputSwitch) {
                            inputSwitch->ToggleState();
                        }
                    } else {
                        selectedComponent = nullptr;
                        currentState = ProgramState::IDLE;
                    }
                }
                break;
            case ProgramState::PLACING_COMPONENT:
                {
                    Component* newComponent = nullptr;
                    switch (currentComponentType) {
                        case ComponentType::AND:
                            newComponent = new AndGate(snappedPosition);
                            break;
                        case ComponentType::OR:
                            newComponent = new OrGate(snappedPosition);
                            break;
                        case ComponentType::NOT:
                            newComponent = new NotGate(snappedPosition);
                            break;
                        case ComponentType::INPUT_SWITCH:
                            newComponent = new InputSwitch(snappedPosition);
                            break;
                    }
                    if (newComponent) {
                        newComponent->SetComponentManager(&ComponentManager::getInstance());
                        newComponent->SetRotation(placementRotation);
                        newComponent->SetScale(camera.zoom);
                        ComponentManager::getInstance().addComponent(newComponent);
                        std::cout << "New component created at: (" << snappedPosition.x << ", " << snappedPosition.y 
                                  << ") with rotation " << placementRotation 
                                  << " and scale " << camera.zoom << std::endl;
                        placementRotation = 0.0f;  // Reset placement rotation after placing component
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

    // Handle wire creation
    if (currentState == ProgramState::SELECTING && selectedComponent) {
        int pinIndex = GetPinAtPosition(selectedComponent, worldMousePos);
        if (pinIndex != -1 && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            wireStartComponent = selectedComponent;
            wireStartPin = pinIndex;
            currentState = ProgramState::CONNECTING_WIRE;
        }
    }

    if (IsKeyPressed(KEY_D)) {
        showDebugInfo = !showDebugInfo;
    }

    if (IsKeyPressed(KEY_F)) {
        Component::ToggleDebugFrames();
    }

    // Handle wire deletion
    if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE)) {
        Wire* wireToDelete = GetWireAtPosition(worldMousePos);
        if (wireToDelete) {
            ConnectionManager::getInstance().removeWire(wireToDelete);
        }
    }

    wireEndPos = worldMousePos;
    prevMousePos = mousePosition;

    // Handle window resizing
    if (IsWindowResized()) {
        int newWidth = GetScreenWidth();
        int newHeight = GetScreenHeight();
        renderer->HandleResize(newWidth, newHeight, camera, ORIGINAL_SCREEN_WIDTH, ORIGINAL_SCREEN_HEIGHT);
        SCREEN_WIDTH = newWidth;
        SCREEN_HEIGHT = newHeight;
    }
}

void Update() {
    // Update all wires
    for (auto& wire : wires) {
        wire->Update();
    }

    // Update all components
    for (auto& component : ComponentManager::getInstance().getComponents()) {
        component->Update();
    }
}

Component* GetComponentAtPosition(Vector2 position) {
    for (auto& component : ComponentManager::getInstance().getComponents()) {
        if (component->IsHovered(position)) {
            return component;
        }
    }
    return nullptr;
}

int GetPinAtPosition(Component* component, Vector2 position) {
    if (!component) return -1;
    
    // Check each pin position
    for (int i = 0; i < component->GetNumInputs() + component->GetNumOutputs(); ++i) {
        Vector2 pinPos = component->GetPinPosition(i);
        if (Vector2Distance(pinPos, position) <= CONNECTION_RADIUS * component->GetScale()) {
            return i;
        }
    }
    return -1;
}

void UpdateWiresForComponent(Component* component) {
    for (auto& wire : wires) {
        if (wire->GetStartComponent() == component || wire->GetEndComponent() == component) {
            wire->UpdateConnectionsAfterRotation();
        }
    }
}

Wire* GetWireAtPosition(Vector2 position) {
    for (auto& wire : wires) {
        if (wire->IsPointOnWire(position)) {
            return wire;
        }
    }
    return nullptr;
}
