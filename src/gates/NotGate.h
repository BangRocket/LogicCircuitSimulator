#ifndef NOT_GATE_H
#define NOT_GATE_H

#include "../core/Component.h"

class NotGate : public Component {
public:
    NotGate(Vector2 position);
    void Update() override;
    void Draw() const override;
    bool IsHovered(Vector2 mousePosition);

    Vector2 GetInputPinPosition(int index) const override;
    Vector2 GetOutputPinPosition(int index) const override;

protected:
    // Using the default size from Component class
};

#endif // NOT_GATE_H
