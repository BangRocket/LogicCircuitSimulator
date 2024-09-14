#include "Grid.h"
#include <cmath>

Vector2 Grid::SnapToGrid(Vector2 position) {
    return {
        floorf(position.x / GRID_SIZE) * GRID_SIZE + GRID_SIZE * 2,
        floorf(position.y / GRID_SIZE) * GRID_SIZE + GRID_SIZE * 2
    };
}

void Grid::Draw(Vector2 screenStart, Vector2 screenEnd) {
    int startX = static_cast<int>(screenStart.x / GRID_SIZE) * GRID_SIZE - GRID_SIZE;
    int startY = static_cast<int>(screenStart.y / GRID_SIZE) * GRID_SIZE - GRID_SIZE;
    int endX = static_cast<int>(screenEnd.x / GRID_SIZE) * GRID_SIZE + GRID_SIZE;
    int endY = static_cast<int>(screenEnd.y / GRID_SIZE) * GRID_SIZE + GRID_SIZE;

    for (int i = startX; i <= endX; i += GRID_SIZE) {
        DrawLineV({static_cast<float>(i), screenStart.y}, {static_cast<float>(i), screenEnd.y}, LIGHTGRAY);
    }
    for (int i = startY; i <= endY; i += GRID_SIZE) {
        DrawLineV({screenStart.x, static_cast<float>(i)}, {screenEnd.x, static_cast<float>(i)}, LIGHTGRAY);
    }
}
