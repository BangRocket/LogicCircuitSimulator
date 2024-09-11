#ifndef NOT_GATE_H
#define NOT_GATE_H

#include "Component.h"

class NotGate : public Component {
public:
    NotGate(Vector2 position);
    void Update() override;
    void Draw() const override;
    Vector2 GetInputPinPosition(int index) const override;
    Vector2 GetOutputPinPosition(int index) const override;
};

#endif // NOT_GATE_H