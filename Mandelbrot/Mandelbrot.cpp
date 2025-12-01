#include <iostream>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#include "mandelbrot_fs.h"
#include "viewport.h"

int main() {
	// Define window dimensions
	const int window_width = 400;
	const int window_height = 400;

	// Create window and GL context
	InitWindow(window_width, window_height, "Raylib");
	SetTargetFPS(60);

	// Load shader
	//Shader shader = LoadShader(NULL, "./shader.fs");
	Shader shader = LoadShaderFromMemory(NULL, mandelbrot_fs);
	int point_loc = GetShaderLocation(shader, "point");
	int mouse_buttons_loc = GetShaderLocation(shader, "mouse_buttons");
	int julia_toggle_loc = GetShaderLocation(shader, "julia_toggle");

	// Viewport
	Viewport viewport;
	viewport.offset = { 300.0f,200.0f };
	viewport.zoom = 200.0f;
	viewport.updateRectangle();

	// Movable point vector
	Vector2 point = { 0.0f,0.0f };
	int show_julia = 0; // Controller for julia_toggle uniform
	
	// Game loop
	while (!WindowShouldClose()) {
		viewport.update();
		Rectangle viewport_rect = viewport.getRectangle();

		int mouse_buttons = IsMouseButtonDown(MOUSE_BUTTON_LEFT) + 2 * IsMouseButtonDown(MOUSE_BUTTON_RIGHT) + 4 * IsMouseButtonDown(MOUSE_BUTTON_MIDDLE); // Get mouse button bitmask
		SetShaderValue(shader, mouse_buttons_loc, &mouse_buttons, RL_SHADER_UNIFORM_INT); // Send bitmask

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) { // Drag point
			point += viewport.getMouseDelta();
			SetShaderValue(shader, point_loc, &point, RL_SHADER_UNIFORM_VEC2);
		}
		if (IsKeyDown(KEY_R)) { // Reset viewport
			point = { 0.0f,0.0f }; // Reset point to 0,0
			SetShaderValue(shader, point_loc, &point, RL_SHADER_UNIFORM_VEC2);
		}
		if (IsKeyPressed(KEY_J)) { // Reset viewport
			show_julia = 1 - show_julia;
			SetShaderValue(shader, julia_toggle_loc, &show_julia, RL_SHADER_UNIFORM_INT);
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
		
		if(!show_julia) DrawCircleV(GetWorldToScreen2D(point,viewport), 3.0f, BLUE);
		
		EndDrawing();
	}

	// Destroy window and GL context
	CloseWindow();

	return 0;
}