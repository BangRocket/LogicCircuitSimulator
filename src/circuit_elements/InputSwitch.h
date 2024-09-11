#ifndef INPUT_SWITCH_H
#define INPUT_SWITCH_H

#include "../core/Component.h"

class InputSwitch : public Component {
public:
    InputSwitch(Vector2 position);
    void Update() override;
    void Draw() const override;
    void ToggleState();

    Vector2 GetOutputPinPosition(int index) const override;

private:
    bool state;
    static const float SWITCH_RADIUS;
};

#endif // INPUT_SWITCH_H