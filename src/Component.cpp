#include "Component.h"

Component::Component(Vector2 position) : position(position) {}

Vector2 Component::GetPosition() const {
    return position;
}

void Component::SetPosition(Vector2 newPosition) {
    position = newPosition;
}
