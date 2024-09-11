#ifndef OR_GATE_H
#define OR_GATE_H

#include "Component.h"

class OrGate : public Component {
public:
    OrGate(Vector2 position);
    void Update() override;
    void Draw() const override;
    Vector2 GetInputPinPosition(int index) const override;
    Vector2 GetOutputPinPosition(int index) const override;
};

#endif // OR_GATE_H