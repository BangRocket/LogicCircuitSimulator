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
void DrawPlaceholderComponent(Vector2 position);
void DrawDebugInfo();

// Global variables
bool isPlacingComponent = false;
Vector2 componentPosition = {0, 0};
bool showDebugInfo = true;

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
        if (isPlacingComponent) {
            isPlacingComponent = false;
        } else {
            isPlacingComponent = true;
            componentPosition = snappedPosition;
        }
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Mouse dragging logic will be implemented here
    }

    if (IsKeyPressed(KEY_U)) {
        // Undo logic will be implemented here
    }

    if (IsKeyPressed(KEY_R)) {
        // Redo logic will be implemented here
    }

    if (IsKeyPressed(KEY_D)) {
        showDebugInfo = !showDebugInfo;
    }

    if (isPlacingComponent) {
        componentPosition = snappedPosition;
    }
}

void Update() {
    // Update logic will be implemented here
}

void Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    DrawGrid();
    
    if (isPlacingComponent) {
        DrawPlaceholderComponent(componentPosition);
    }
    
    DrawText("Logic Circuit Simulator", 190, 200, 20, LIGHTGRAY);
    
    if (showDebugInfo) {
        DrawDebugInfo();
    }
    
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

void DrawPlaceholderComponent(Vector2 position) {
    DrawRectangle(position.x, position.y, GRID_SIZE, GRID_SIZE, BLUE);
    DrawRectangleLines(position.x, position.y, GRID_SIZE, GRID_SIZE, DARKBLUE);
}
void DrawDebugInfo() {
    Vector2 mousePosition = GetMousePosition();
    Vector2 snappedPosition = SnapToGrid(mousePosition);
    DrawText(TextFormat("Mouse Position: (%.0f, %.0f)", snappedPosition.x, snappedPosition.y), 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Grid Position: (%d, %d)", (int)snappedPosition.x / GRID_SIZE, (int)snappedPosition.y / GRID_SIZE), 10, 40, 20, DARKGRAY);
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 70, 20, DARKGRAY);
    DrawText(TextFormat("Placing Component: %s", isPlacingComponent ? "Yes" : "No"), 10, 100, 20, DARKGRAY);
}
