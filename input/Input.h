#ifndef INPUT_H
#define INPUT_H

#include "../core/GameState.h"
#include "../core/Component.h"
#include "../rendering/Renderer.h"
#include <vector>

class Input {
public:
    static void HandleInput(ProgramState& currentState, 
                            ComponentType& currentComponentType,
                            Component*& wireStartComponent,
                            int& wireStartPin,
                            Vector2& wireEndPos,
                            bool& showDebugInfo,
                            Component*& selectedComponent,
                            float& placementRotation,
                            Camera2D& camera,
                            Renderer* renderer,
                            std::vector<Wire*>& wires);

private:
    static Component* GetComponentAtPosition(Vector2 position);
    static int GetPinAtPosition(Component* component, Vector2 position);
    static void UpdateWiresForComponent(Component* component);
    static Wire* GetWireAtPosition(Vector2 position);
};

#endif // INPUT_H
