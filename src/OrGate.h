#ifndef OR_GATE_H
#define OR_GATE_H

#include "Component.h"

class OrGate : public Component {
public:
    OrGate(Vector2 position);
    void Update() override;
};

#endif // OR_GATE_H