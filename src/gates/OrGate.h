#ifndef OR_GATE_H
#define OR_GATE_H

#include "../core/Component.h"

class OrGate : public Component {
public:
    OrGate(Vector2 position);
    void Update() override;
    void Draw() const override;
    bool IsHovered(Vector2 mousePosition);

protected:
    // Using the default size from Component class
};

#endif // OR_GATE_H
