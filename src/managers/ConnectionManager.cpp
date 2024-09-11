#include "ConnectionManager.h"
#include "../circuit_elements/Wire.h"
#include "raymath.h"

const float ConnectionManager::SNAP_DISTANCE = 10.0f;

ConnectionManager& ConnectionManager::getInstance() {
    static ConnectionManager instance;
    return instance;
}

ConnectionManager::~ConnectionManager() {
    for (Wire* wire : wires) {
        delete wire;
    }
}

void ConnectionManager::addWire(Wire* wire) {
    wires.push_back(wire);
    addWireToComponent(wire->GetStartComponent(), wire);
    addWireToComponent(wire->GetEndComponent(), wire);
}

void ConnectionManager::removeWire(Wire* wire) {
    auto it = std::find(wires.begin(), wires.end(), wire);
    if (it != wires.end()) {
        removeWireFromComponent(wire->GetStartComponent(), wire);
        removeWireFromComponent(wire->GetEndComponent(), wire);
        wires.erase(it);
        delete wire;
    }
}

void ConnectionManager::updateConnections() {
    for (Wire* wire : wires) {
        wire->Update();
    }
}

Wire* ConnectionManager::getWireAtPin(const Component* component, int pinIndex) const {
    auto it = componentWires.find(component);
    if (it != componentWires.end()) {
        for (Wire* wire : it->second) {
            if ((wire->GetStartComponent() == component && wire->GetStartPinIndex() == pinIndex) ||
                (wire->GetEndComponent() == component && wire->GetEndPinIndex() == pinIndex)) {
                return wire;
            }
        }
    }
    return nullptr;
}

bool ConnectionManager::canConnect(const Component* startComponent, int startPinIndex, const Component* endComponent, int endPinIndex) const {
    // Check if there's already a wire connected to either pin
    if (getWireAtPin(startComponent, startPinIndex) || getWireAtPin(endComponent, endPinIndex)) {
        return false;
    }

    // Check if we're trying to connect an output to an input
    bool startIsOutput = startPinIndex >= startComponent->GetNumInputs();
    bool endIsInput = endPinIndex < endComponent->GetNumInputs();

    return startIsOutput != endIsInput;
}

Wire* ConnectionManager::createConnection(Component* startComponent, int startPinIndex, Component* endComponent, int endPinIndex) {
    if (canConnect(startComponent, startPinIndex, endComponent, endPinIndex)) {
        Wire* newWire = new Wire(startComponent, startPinIndex, endComponent, endPinIndex);
        addWire(newWire);
        return newWire;
    }
    return nullptr;
}

void ConnectionManager::updateComponentPosition(Component* component, const Vector2& newPosition) {
    component->SetPosition(newPosition);
    auto it = componentWires.find(component);
    if (it != componentWires.end()) {
        for (Wire* wire : it->second) {
            wire->Update();
        }
    }
}

bool ConnectionManager::isSnappable(const Vector2& pos1, const Vector2& pos2) const {
    return Vector2Distance(pos1, pos2) <= SNAP_DISTANCE;
}

void ConnectionManager::addWireToComponent(const Component* component, Wire* wire) {
    componentWires[component].push_back(wire);
}

void ConnectionManager::removeWireFromComponent(const Component* component, Wire* wire) {
    auto it = componentWires.find(component);
    if (it != componentWires.end()) {
        auto wireIt = std::find(it->second.begin(), it->second.end(), wire);
        if (wireIt != it->second.end()) {
            it->second.erase(wireIt);
        }
    }
}