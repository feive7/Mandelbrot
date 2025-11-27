#include <iostream>
#include <raylib.h>
#include <rlgl.h>

int main() {
	// Define window dimensions
	const int window_width = 400;
	const int window_height = 400;

	// Create window and GL context
	InitWindow(window_width, window_height, "Raylib");
	SetTargetFPS(60);

	// Load shader
	Shader shader = LoadShader(NULL, "./shader.fs");

	// Game loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		
		ClearBackground(RAYWHITE);
		
		BeginShaderMode(shader);
		
		rlBegin(RL_QUADS);
		
		rlTexCoord2f(0.0f, 0.0f); rlVertex2f(0.0f, 0.0f);
		rlTexCoord2f(0.0f, 1.0f); rlVertex2f(0.0f, window_height);
		rlTexCoord2f(1.0f, 1.0f); rlVertex2f(window_width, window_height);
		rlTexCoord2f(1.0f, 0.0f); rlVertex2f(window_width, 0.0f);

		EndShaderMode();
		
		EndDrawing();
	}

	// Destroy window and GL context
	CloseWindow();

	return 0;
}