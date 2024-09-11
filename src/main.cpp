#include "raylib.h"
#include <iostream>

// Function prototypes
void HandleInput();
void Update();
void Render();

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Logic Circuit Simulator");
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
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        std::cout << "Left mouse button clicked at: (" << mousePosition.x << ", " << mousePosition.y << ")" << std::endl;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        std::cout << "Mouse dragging at: (" << mousePosition.x << ", " << mousePosition.y << ")" << std::endl;
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
    
    DrawText("Logic Circuit Simulator", 190, 200, 20, LIGHTGRAY);
    
    Vector2 mousePosition = GetMousePosition();
    DrawText(TextFormat("Mouse Position: (%.0f, %.0f)", mousePosition.x, mousePosition.y), 10, 10, 20, DARKGRAY);
    
    EndDrawing();
}
