#ifndef AND_GATE_H
#define AND_GATE_H

#include "../core/Component.h"

class AndGate : public Component {
public:
    AndGate(Vector2 position);
    void Update() override;
    void Draw() const override;
    bool IsHovered(Vector2 mousePosition);

protected:
    // Using the default size from Component class
};

#endif // AND_GATE_H
