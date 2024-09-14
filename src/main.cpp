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
#include "input/Input.h"
#include <iostream>
#include <vector>
#include <raymath.h>

// Constants
const int ORIGINAL_SCREEN_WIDTH = 1024;
const int ORIGINAL_SCREEN_HEIGHT = 768;

int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;

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

// Renderer instance
Renderer* renderer = nullptr;

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
        Input::HandleInput(currentState, currentComponentType, wireStartComponent, wireStartPin, wireEndPos, showDebugInfo, selectedComponent, placementRotation, camera, renderer, wires);
        Update();
        Vector2 mousePosition = GetMousePosition();
        Vector2 worldMousePos = renderer->ScreenToWorld(mousePosition);
        renderer->Render(currentState, wireStartComponent, wireStartPin, wireEndPos, showDebugInfo, selectedComponent, currentComponentType, placementRotation, mousePosition, worldMousePos);
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
