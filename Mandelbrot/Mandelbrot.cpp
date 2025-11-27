#include <iostream>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#include "viewport.h"

int main() {
	// Define window dimensions
	const int window_width = 400;
	const int window_height = 400;

	// Create window and GL context
	InitWindow(window_width, window_height, "Raylib");
	SetTargetFPS(60);

	// Load shader
	Shader shader = LoadShader(NULL, "./shader.fs");

	// Viewport
	Viewport viewport;
	viewport.offset = { 300.0f,200.0f };
	viewport.zoom = 200.0f;
	viewport.updateRectangle();
	
	// Game loop
	while (!WindowShouldClose()) {
		viewport.update();
		Rectangle viewport_rect = viewport.getRectangle();

		BeginDrawing();
		
		ClearBackground(RAYWHITE);
		
		BeginShaderMode(shader);
		
		rlBegin(RL_QUADS);
		
		rlTexCoord2f(viewport_rect.x, viewport_rect.y); rlVertex2f(0.0f, 0.0f);
		rlTexCoord2f(viewport_rect.x, viewport_rect.y + viewport_rect.height); rlVertex2f(0.0f, window_height);
		rlTexCoord2f(viewport_rect.x + viewport_rect.width, viewport_rect.y + viewport_rect.height); rlVertex2f(window_width, window_height);
		rlTexCoord2f(viewport_rect.x + viewport_rect.width, viewport_rect.y); rlVertex2f(window_width, 0.0f);

		EndShaderMode();
		
		EndDrawing();
	}

	// Destroy window and GL context
	CloseWindow();

	return 0;
}