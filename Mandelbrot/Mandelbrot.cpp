#include <iostream>
#include <raylib.h>

int main() {
	// Define window dimensions
	const int window_width = 400;
	const int window_height = 400;

	// Create window and GL context
	InitWindow(window_width, window_height, "Raylib");
	SetTargetFPS(60);

	// Game loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Hello World!", 5, 5, 20, BLACK);
		EndDrawing();
	}

	// Destroy window and GL context
	CloseWindow();

	return 0;
}