#include "multiple_simulator.hpp"

bool is_cursor_in_x(const Region & region, const Vector2 & mouse){
	if (mouse.x > region.x_min &&
	    mouse.x < region.x_max &&
	    mouse.y > region.y_min &&
	    mouse.y < region.y_max){
		return true;	
	}
	return false;

}

bool is_cursor_on_button(const Button & button, const Vector2 & mouse){
	if (mouse.x > button.x_min &&
	    mouse.x < button.x_max &&
	    mouse.y > button.y_min &&
	    mouse.y < button.y_max){
		return true;
	}
	return false;
}

void get_input(Camera2D & camera){
	
	Vector2 mouse = GetMousePosition();
	mouse_was_clicked = mouse_is_clicked;
	mouse_is_clicked = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

	// following block is for key-based commands
	int key = GetKeyPressed();
	switch(key){
		case KEY_H:
			highlight_interactables = !highlight_interactables;
			break;
	}

	if (is_cursor_in_x(regions.at("world"), mouse)){
		if (resistor_being_added &&
			!mouse_is_clicked &&
			mouse_was_clicked){
			resistors.push_back({
				std::string("R") + std::to_string(resistors.size() + 1), 
				12 * (int)(mouseWorldPos.x / 12),
				12 * (int)(mouseWorldPos.y / 12) + 6,
				EAST,
				1000
			});
			resistor_being_added = false;
			world_buttons.push_back({
				resistors[resistors.size() - 1].name,
				resistors[resistors.size() - 1].x,
				resistors[resistors.size() - 1].x + 60,
				resistors[resistors.size() - 1].y - 30,
				resistors[resistors.size() - 1].y + 30,
				false	
			});
		}
		else if (source_being_added &&
			!mouse_is_clicked &&
			mouse_was_clicked){
			sources.push_back({
				std::string("V") + std::to_string(sources.size() + 1),
				12 * (int)(mouseWorldPos.x / 12) - 5,
				12 * (int)(mouseWorldPos.y / 12),
				NORTH,
				5
			});
			source_being_added = false;
		}
		else if (agilent_being_added &&
			!mouse_is_clicked &&
			mouse_was_clicked){
			DMMs.push_back({
				std::string("DMM") + std::to_string(DMMs.size() + 1),
				12 * (int)(mouseWorldPos.x / 12),
				12 * (int)(mouseWorldPos.y / 12)
			});
			agilent_being_added = false;
		}


		if (mouse_is_clicked){
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f/camera.zoom);
			camera.target = Vector2Add(camera.target, delta);
		}
		float wheel = GetMouseWheelMove();
		if (wheel != 0){
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
	else if(is_cursor_in_x(regions.at("place_component_prompt"), mouse)){}

	else if(is_cursor_on_button(buttons[0], mouse)){ // new source
		if (mouse_was_clicked){
			region_should_darken = {
				true,
				buttons[0].x_min,
				buttons[0].y_min,
				buttons[0].x_max,
				buttons[0].y_max
			};
			if (!mouse_is_clicked)
				source_being_added = true;
		}
	}
	else if(is_cursor_on_button(buttons[1], mouse)){ // new resistor
		if (mouse_was_clicked){
			region_should_darken = {
				true,
				buttons[1].x_min,
				buttons[1].y_min,
				buttons[1].x_max,
				buttons[1].y_max
			};
			if (!mouse_is_clicked)
				resistor_being_added = true;
		}
	}
	else if(is_cursor_on_button(buttons[2], mouse)){ // new agilent DMM
		if (mouse_was_clicked){
			region_should_darken = {
				true,
				buttons[2].x_min,
				buttons[2].y_min,
				buttons[2].x_max,
				buttons[2].y_max
			};
			if (!mouse_is_clicked)
				agilent_being_added = true;
		}
	}
}
