#ifndef AND_GATE_H
#define AND_GATE_H

#include "Component.h"

class AndGate : public Component {
public:
    AndGate(Vector2 position);
    void Update() override;
};

#endif // AND_GATE_H