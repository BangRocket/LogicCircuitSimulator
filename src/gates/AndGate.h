#ifndef AND_GATE_H
#define AND_GATE_H

#include "../core/Component.h"

class AndGate : public Component {
public:
    AndGate(Vector2 position);
    void Update() override;
    void Draw() const override;
	bool IsHovered(Vector2 mousePosition);
	Vector2 GetInputPinPosition(int index) const override;
	Vector2 GetOutputPinPosition(int index) const override;

protected:
    // Override the default size if needed
    // Using the default size from Component class
};

#endif // AND_GATE_H
