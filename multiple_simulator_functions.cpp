#include "multiple_simulator.hpp"

bool is_cursor_in_x(const Region & region){
	Vector2 mouse = GetMousePosition();
	if (mouse.x > region.x_top &&
	    mouse.x < region.x_bottom &&
	    mouse.y > region.y_top &&
	    mouse.y < region.y_bottom){
		return true;	
	}
	return false;

}

void get_input(Camera2D & camera, const std::unordered_map<std::string, Region>& Regions, Region_should_darken & region_should_darken){
	// Moving within world environment
	if (is_cursor_in_x(Regions.at("world"))){
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f/camera.zoom);
			camera.target = Vector2Add(camera.target, delta);
		}
		float wheel = GetMouseWheelMove();
		if (wheel != 0){
			Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
			camera.offset = GetMousePosition();
			camera.target = mouseWorldPos;
			float scale = 0.2f * wheel;
			camera.zoom = Clamp(expf(logf(camera.zoom) + scale),
				0.125f,
				64.0f
			);
		}
	}
	// Opening component menu
	else if(is_cursor_in_x(Regions.at("place_component_prompt"))){
		Region temp = Regions.at("place_component_prompt");
		region_should_darken = {
			1,
			temp.x_top,
			temp.y_top,
			temp.x_bottom,
			temp.y_bottom
		};
	}
}
