#include "raylib.h"
#include "core/Component.h"
#include "gates/AndGate.h"
#include "gates/OrGate.h"
#include "gates/NotGate.h"
#include "utils/Wire.h"
#include "utils/ResourceManager.h"
#include <iostream>
#include <vector>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 32;

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
    IDLE
};

// Function prototypes
void HandleInput();
void Update();
void Render();
void DrawGrid();
Vector2 SnapToGrid(Vector2 position);
void DrawDebugInfo();
Component* GetComponentAtPosition(Vector2 position);
int GetPinAtPosition(Component* component, Vector2 position);

// Global variables
ProgramState currentState = ProgramState::IDLE;
ComponentType currentComponentType = ComponentType::AND;
std::vector<Component*> components;
std::vector<Wire*> wires;
Component* wireStartComponent = nullptr;
int wireStartPin = -1;
Vector2 wireEndPos = {0, 0};
bool showDebugInfo = true;

// New InputSwitch class
class InputSwitch : public Component {
public:
    InputSwitch(Vector2 position) : Component(position, "input_switch", 0, 1), state(false) {}

    void Update() override {
        outputStates[0] = state;
    }

    void Draw() const override {
        Component::Draw();
        DrawRectangle(position.x + 16, position.y + 16, 32, 32, state ? GREEN : RED);
    }

    void ToggleState() {
        state = !state;
    }

private:
    bool state;
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Logic Circuit Simulator");
    SetTargetFPS(60);

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
    Vector2 snappedPosition = SnapToGrid(mousePosition);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (currentState) {
            case ProgramState::IDLE:
                {
                    Component* clickedComponent = GetComponentAtPosition(mousePosition);
                    if (clickedComponent) {
                        int pinIndex = GetPinAtPosition(clickedComponent, mousePosition);
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
                        components.push_back(newComponent);
                    }
                    currentState = ProgramState::IDLE;
                }
                break;
            case ProgramState::CONNECTING_WIRE:
                {
                    Component* endComponent = GetComponentAtPosition(mousePosition);
                    if (endComponent && endComponent != wireStartComponent) {
                        int endPin = GetPinAtPosition(endComponent, mousePosition);
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

    if (IsKeyPressed(KEY_A)) {
        currentComponentType = ComponentType::AND;
    }
    if (IsKeyPressed(KEY_O)) {
        currentComponentType = ComponentType::OR;
    }
    if (IsKeyPressed(KEY_N)) {
        currentComponentType = ComponentType::NOT;
    }
    if (IsKeyPressed(KEY_I)) {
        currentComponentType = ComponentType::INPUT_SWITCH;
    }

    if (IsKeyPressed(KEY_D)) {
        showDebugInfo = !showDebugInfo;
    }

    wireEndPos = mousePosition;
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
    
    if (showDebugInfo) {
        DrawDebugInfo();
    }
    
    EndDrawing();
}

void DrawGrid() {
    for (int i = 0; i < SCREEN_WIDTH; i += GRID_SIZE) {
        DrawLine(i, 0, i, SCREEN_HEIGHT, LIGHTGRAY);
    }
    for (int i = 0; i < SCREEN_HEIGHT; i += GRID_SIZE) {
        DrawLine(0, i, SCREEN_WIDTH, i, LIGHTGRAY);
    }
}

Vector2 SnapToGrid(Vector2 position) {
    return {
        float(int(position.x / GRID_SIZE) * GRID_SIZE),
        float(int(position.y / GRID_SIZE) * GRID_SIZE)
    };
}

void DrawDebugInfo() {
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Components: %d", (int)components.size()), 10, 40, 20, DARKGRAY);
    DrawText(TextFormat("Wires: %d", (int)wires.size()), 10, 70, 20, DARKGRAY);
    DrawText(TextFormat("State: %s", 
        currentState == ProgramState::IDLE ? "IDLE" :
        currentState == ProgramState::PLACING_COMPONENT ? "PLACING" : "CONNECTING"), 10, 100, 20, DARKGRAY);
    DrawText(TextFormat("Current Component: %s", 
        currentComponentType == ComponentType::AND ? "AND" : 
        currentComponentType == ComponentType::OR ? "OR" : 
        currentComponentType == ComponentType::NOT ? "NOT" : "INPUT"), 10, 130, 20, DARKGRAY);
}

Component* GetComponentAtPosition(Vector2 position) {
    for (auto& component : components) {
        Vector2 compPos = component->GetPosition();
        if (position.x >= compPos.x && position.x < compPos.x + 64 &&
            position.y >= compPos.y && position.y < compPos.y + 64) {
            return component;
        }
    }
    return nullptr;
}

int GetPinAtPosition(Component* component, Vector2 position) {
    if (!component) return -1;

    for (int i = 0; i < component->GetNumInputs() + component->GetNumOutputs(); ++i) {
        Vector2 pinPos = component->GetPinPosition(i);
        if (CheckCollisionPointCircle(position, pinPos, 5)) {
            return i;
        }
    }
    return -1;
}
