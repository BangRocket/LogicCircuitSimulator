#include "Input.h"
#include "../managers/ComponentManager.h"
#include "../managers/ConnectionManager.h"
#include "../gates/AndGate.h"
#include "../gates/OrGate.h"
#include "../gates/NotGate.h"
#include "../circuit_elements/InputSwitch.h"
#include <iostream>
#include <raymath.h>

void Input::HandleInput(ProgramState& currentState, 
                        ComponentType& currentComponentType,
                        Component*& wireStartComponent,
                        int& wireStartPin,
                        Vector2& wireEndPos,
                        bool& showDebugInfo,
                        Component*& selectedComponent,
                        float& placementRotation,
                        Camera2D& camera,
                        Renderer* renderer,
                        std::vector<Wire*>& wires) {
    Vector2 mousePosition = GetMousePosition();
    Vector2 worldMousePos = renderer->ScreenToWorld(mousePosition);
    Vector2 snappedPosition = renderer->SnapToGrid(worldMousePos);

    // Handle toolbar interactions
    if (mousePosition.y < renderer->GetToolbarHeight()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int buttonWidth = GetScreenWidth() / 4;
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
            placementRotation += 90.0f;
            if (placementRotation >= 360.0f) placementRotation -= 360.0f;
        } else if (selectedComponent) {
            selectedComponent->Rotate(90.0f);
            UpdateWiresForComponent(selectedComponent);
        }
    }

    // Handle panning with right mouse button
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 mouseDelta = GetMouseDelta();
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
            case ProgramState::PANNING:
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

                        // Handle component dragging
                        HandleComponentDragging(clickedComponent, worldMousePos, renderer);
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
    static Wire* highlightedWire = nullptr;
    Wire* wireUnderMouse = GetWireAtPosition(worldMousePos);
    
    if (wireUnderMouse != highlightedWire) {
        highlightedWire = wireUnderMouse;
        renderer->HighlightWireForDeletion(highlightedWire);
    }

    if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE)) {
        if (highlightedWire) {
            ConnectionManager::getInstance().removeWire(highlightedWire);
            highlightedWire = nullptr;
            renderer->HighlightWireForDeletion(nullptr);
        }
    }

    wireEndPos = worldMousePos;

    // Handle window resizing
    if (IsWindowResized()) {
        int newWidth = GetScreenWidth();
        int newHeight = GetScreenHeight();
        renderer->HandleResize(newWidth, newHeight, camera, 800, 600);
    }
}

Component* Input::GetComponentAtPosition(Vector2 position) {
    for (auto& component : ComponentManager::getInstance().getComponents()) {
        if (component->IsHovered(position)) {
            return component;
        }
    }
    return nullptr;
}

int Input::GetPinAtPosition(Component* component, Vector2 position) {
    if (!component) return -1;
    
    // Check each pin position
    for (int i = 0; i < component->GetNumInputs() + component->GetNumOutputs(); ++i) {
        Vector2 pinPos = component->GetPinPosition(i);
        if (Vector2Distance(pinPos, position) <= 20.0f * component->GetScale()) {
            return i;
        }
    }
    return -1;
}

void Input::UpdateWiresForComponent(Component* component) {
    for (auto& wire : ConnectionManager::getInstance().getWires()) {
        if (wire->GetStartComponent() == component || wire->GetEndComponent() == component) {
            wire->UpdateConnectionsAfterRotation();
        }
    }
}

Wire* Input::GetWireAtPosition(Vector2 position) {
    for (auto& wire : ConnectionManager::getInstance().getWires()) {
        if (wire->IsPointOnWire(position)) {
            return wire;
        }
    }
    return nullptr;
}

void Input::HandleComponentDragging(Component*& selectedComponent, Vector2 worldMousePos, Renderer* renderer) {
    if (selectedComponent && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 snappedPosition = renderer->SnapToGrid(worldMousePos);
        selectedComponent->SetPosition(snappedPosition);
        UpdateWiresForComponent(selectedComponent);
    }
}
