#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include "../circuit_elements/Wire.h"
#include "../core/Component.h"
#include <vector>
#include <unordered_map>

class ConnectionManager {
public:
    static ConnectionManager& getInstance();

    void addWire(Wire* wire);
    void removeWire(Wire* wire);
    void updateConnections();
    Wire* getWireAtPin(const Component* component, int pinIndex) const;
    bool canConnect(const Component* startComponent, int startPinIndex, const Component* endComponent, int endPinIndex) const;
    Wire* createConnection(Component* startComponent, int startPinIndex, Component* endComponent, int endPinIndex);

    void updateComponentPosition(Component* component, const Vector2& newPosition);

    const std::vector<Wire*>& getWires() const { return wires; }

private:
    ConnectionManager() = default;
    ~ConnectionManager();
    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    std::vector<Wire*> wires;
    std::unordered_map<const Component*, std::vector<Wire*>> componentWires;

    static const float SNAP_DISTANCE;

    bool isSnappable(const Vector2& pos1, const Vector2& pos2) const;
    void addWireToComponent(const Component* component, Wire* wire);
    void removeWireFromComponent(const Component* component, Wire* wire);
};

#endif // CONNECTION_MANAGER_H
