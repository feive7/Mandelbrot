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
	int point_loc = GetShaderLocation(shader, "point");

	// Viewport
	Viewport viewport;
	viewport.offset = { 300.0f,200.0f };
	viewport.zoom = 200.0f;
	viewport.updateRectangle();

	// Movable point vector
	Vector2 point = { 0.0f,0.0f };
	
	// Game loop
	while (!WindowShouldClose()) {
		viewport.update();
		Rectangle viewport_rect = viewport.getRectangle();

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			point += viewport.getMouseDelta();
			SetShaderValue(shader, point_loc, &point, RL_SHADER_UNIFORM_VEC2);
		}
		if (IsKeyDown(KEY_R)) {
			point = { 0.0f,0.0f }; // Reset point to 0,0
			SetShaderValue(shader, point_loc, &point, RL_SHADER_UNIFORM_VEC2);
		}

		BeginDrawing();
		
		ClearBackground(RAYWHITE);
		
		BeginShaderMode(shader);
		
		rlBegin(RL_QUADS);
		
		rlTexCoord2f(viewport_rect.x, viewport_rect.y); rlVertex2f(0.0f, 0.0f);
		rlTexCoord2f(viewport_rect.x, viewport_rect.y + viewport_rect.height); rlVertex2f(0.0f, window_height);
		rlTexCoord2f(viewport_rect.x + viewport_rect.width, viewport_rect.y + viewport_rect.height); rlVertex2f(window_width, window_height);
		rlTexCoord2f(viewport_rect.x + viewport_rect.width, viewport_rect.y); rlVertex2f(window_width, 0.0f);

		EndShaderMode();
		
		DrawCircleV(GetWorldToScreen2D(point,viewport), 3.0f, BLUE);
		
		EndDrawing();
	}

	// Destroy window and GL context
	CloseWindow();

	return 0;
}