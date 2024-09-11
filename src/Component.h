#ifndef COMPONENT_H
#define COMPONENT_H

#include "raylib.h"

class Component {
public:
    Component(Vector2 position);
    virtual ~Component() = default;

    virtual void Update() = 0;
    virtual void Draw() const = 0;

    Vector2 GetPosition() const;
    void SetPosition(Vector2 newPosition);

protected:
    Vector2 position;
};

#endif // COMPONENT_H
