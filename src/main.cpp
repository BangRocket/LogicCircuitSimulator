#include "raylib.h"
#include <iostream>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 32;

// Function prototypes
void HandleInput();
void Update();
void Render();
void DrawGrid();
Vector2 SnapToGrid(Vector2 position);

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Logic Circuit Simulator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        HandleInput();
        Update();
        Render();
    }

    CloseWindow();
    return 0;
}

void HandleInput() {
    Vector2 mousePosition = GetMousePosition();
    Vector2 snappedPosition = SnapToGrid(mousePosition);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        std::cout << "Left mouse button clicked at: (" << snappedPosition.x << ", " << snappedPosition.y << ")" << std::endl;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        std::cout << "Mouse dragging at: (" << snappedPosition.x << ", " << snappedPosition.y << ")" << std::endl;
    }

    if (IsKeyPressed(KEY_U)) {
        std::cout << "Undo shortcut pressed" << std::endl;
    }

    if (IsKeyPressed(KEY_R)) {
        std::cout << "Redo shortcut pressed" << std::endl;
    }
}

void Update() {
    // Update logic will be implemented here
}

void Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    DrawGrid();
    
    DrawText("Logic Circuit Simulator", 190, 200, 20, LIGHTGRAY);
    
    Vector2 mousePosition = GetMousePosition();
    Vector2 snappedPosition = SnapToGrid(mousePosition);
    DrawText(TextFormat("Mouse Position: (%.0f, %.0f)", snappedPosition.x, snappedPosition.y), 10, 10, 20, DARKGRAY);
    
    EndDrawing();
}

void DrawGrid() {
    for (int i = 0; i < SCREEN_WIDTH; i += GRID_SIZE) {
        DrawLine(i, 0, i, SCREEN_HEIGHT, LIGHTGRAY);
    }
    for (int i = 0; i < SCREEN_HEIGHT; i += GRID_SIZE) {
        DrawLine(0, i, SCREEN_WIDTH, i, LIGHTGRAY);
    }
}

Vector2 SnapToGrid(Vector2 position) {
    return {
        float(int(position.x / GRID_SIZE) * GRID_SIZE),
        float(int(position.y / GRID_SIZE) * GRID_SIZE)
    };
}
