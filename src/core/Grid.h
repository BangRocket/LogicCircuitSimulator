#ifndef GRID_H
#define GRID_H

#include "raylib.h"

class Grid {
public:
    static const int GRID_SIZE = 32;

    static Vector2 SnapToGrid(Vector2 position);
    static void Draw(Vector2 screenStart, Vector2 screenEnd);
};

#endif // GRID_H
