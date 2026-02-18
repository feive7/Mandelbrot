#include <raymath.h>
class Viewport : public Camera2D {
public:
    Viewport() {
        offset = { 0.0f,0.0f };
        rotation = 0.0f;
        target = { 0.0f,0.0f };
        zoom = 1.0f;
        updateRectangle(); // Calculate internal values
    }
    Camera2D getThisCamera() {
        Camera2D output;
        output.offset = this->offset;
        output.rotation = this->rotation;
        output.target = this->target;
        output.zoom = this->zoom;
        return output;
    }
    void update(bool override_move_check = false) {
        just_moved = false;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / zoom);
            target = Vector2Add(target, delta);
            just_moved = true;
        }
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            // Get the world point that is under the mouse
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), getThisCamera());

            // Set the offset to where the mouse is
            offset = GetMousePosition();

            // Set the target to match, so that the camera maps the world space point
            // under the cursor to the screen space point under the cursor at any zoom
            target = mouseWorldPos;

            // Zoom increment
            // Uses log scaling to provide consistent zoom speed
            float scale = 0.2f * wheel;
            zoom = expf(logf(zoom) + scale);
            zoom = Clamp(zoom, 0.0f, 1200000.0f);
            just_moved = true;
        }
        if(just_moved || override_move_check) updateRectangle();
    }

    Vector2 mapPoint(Vector2 screen_pos) {
        return GetScreenToWorld2D(screen_pos, getThisCamera());
    }
    Vector2 getMousePosition() {
        return GetScreenToWorld2D(GetMousePosition(), getThisCamera());
    }
    Vector2 getMouseDelta() {
        return GetMouseDelta() / this->zoom;
    }

    Rectangle mapRect(Rectangle rect) {
        Vector2 point1 = mapPoint({ rect.x,rect.y });
        Vector2 point2 = mapPoint({ rect.x + rect.width,rect.y + rect.height });
        return { point1.x,point1.y,point2.x - point1.x,point2.y - point1.y };
    }
    Rectangle getRectangle() {
        return view_rec;
    }

    float getStep() {
        Vector2 a = mapPoint({ 0.0f,0.0f });
        Vector2 b = mapPoint({ 1.0f,0.0f });
        return b.x - a.x;
    }
    bool justMoved() {
        return just_moved;
    }

    void drawPoint(Vector2 point, Color color, float radius = 10.0f) {
        DrawCircleV(point, radius / this->zoom, color);
    }
public:
    bool just_moved = false;
    Rectangle view_rec;
    void updateRectangle() {
        Vector2 min_point = mapPoint({ 0.0f,0.0f });
        Vector2 max_point = mapPoint({ (float)GetScreenWidth(), (float)GetScreenHeight() });
        view_rec = { min_point.x,min_point.y,max_point.x - min_point.x,max_point.y - min_point.y };
    }
};
