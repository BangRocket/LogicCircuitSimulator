#ifndef NOT_GATE_H
#define NOT_GATE_H

#include "Component.h"

class NotGate : public Component {
public:
    NotGate(Vector2 position);
    void Update() override;
};

#endif // NOT_GATE_H