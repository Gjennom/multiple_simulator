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

bool is_point_valid_node_connect(int x, int y){
	Vector2 xy2 = nearest_grid_input((Vector2){(float)x, (float)y});
	x = xy2.x;
	y = xy2.y;
	for (auto element : resistors){
		if (element.connections.c1_x == x && element.connections.c1_y == y) return true;
		else if (element.connections.c2_x == x && element.connections.c2_y == y) return true;
	}
	for (auto element : sources){
		if (element.connections.c1_x == x && element.connections.c1_y == y) return true;
		else if (element.connections.c2_x == x && element.connections.c2_y == y) return true;
	}
	for (auto element : DMMs){
		if (element.connections.c1_x == x && element.connections.c1_y == y) return true;
		else if (element.connections.c2_x == x && element.connections.c2_y == y) return true;
		else if (element.connections.c3_x == x && element.connections.c3_y == y) return true;
		else if (element.connections.c4_x == x && element.connections.c4_y == y) return true;
		else if (element.connections.c5_x == x && element.connections.c5_y == y) return true;
	}
	for (auto connection : connections){
		for (auto element : connection){
		if (element.connections.c1_x == x && element.connections.c1_y == y) return true;
		else if (element.connections.c2_x == x && element.connections.c2_y == y) return true;
		else if ((element.connections.c1_x == element.connections.c2_x == x) || 
			(element.connections.c1_y == element.connections.c2_y == y)){
			Vector2 pos = nearest_grid_input((Vector2){(float)x, (float)y});
			nodes.push_back({
				pos.x,
				pos.y,
				0,
				default_connection
			});
			return true;
		}
	}
	}
	std::cout << "point " << x << ' ' << y << " wasnt valid node" << '\n';
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

void update_resistor_connections(Resistor & resistor){
	Vector2 nearest = nearest_grid_input((Vector2){
		(float)resistor.x, (float)resistor.y
	});
	int x = nearest.x;
	int y = nearest.y;
	Orientation direction = resistor.orientation;
	Connection_points & connections = resistor.connections;
	switch(direction){
		case NORTH:
			connections.c1_x = x;
			connections.c1_y = y;
			connections.c2_x = x;
			connections.c2_y = y + 60;
			break;
		case SOUTH:
			connections.c1_x = x;
			connections.c1_y = y + 6;
			connections.c2_x = x;
			connections.c2_y = y + 66;
			break;
		case EAST:
			connections.c1_x = x;
			connections.c1_y = y + 6;
			connections.c2_x = x + 60;
			connections.c2_y = y + 6;
			break;
		case WEST:
			connections.c1_x = x;
			connections.c1_y = y;
			connections.c2_x = x + 60; 
			connections.c2_y = y;
			break;
	}
	resistor.connections = connections;
}

void update_source_connections(Source & source){
	int x = source.x;
	int y = source.y;
	Orientation direction = source.orientation;
	Connection_points connections = source.connections;
	switch(direction){
		case NORTH:
			connections.c1_x = x + 17;
			connections.c1_y = y;
			connections.c2_x = x + 17;
			connections.c2_y = y + 48;
			break;
		case SOUTH:
			connections.c1_x = x;
			connections.c1_y = y;
			connections.c2_x = x;
			connections.c2_y = y - 60;
			break;
		case EAST:
			connections.c1_x = x;
			connections.c1_y = y;
			connections.c2_x = x + 60;
			connections.c2_y = y;
			break;
		case WEST:
			connections.c1_x = x;
			connections.c1_y = y;
			connections.c2_x = x + 60;
			connections.c2_y = y;
			break;
	}
	source.connections = connections;
}

void update_dmm_connections(DMM & dmm){
	float x = dmm.x;
	float y = dmm.y;
	std::cout << x << ' ' << y << '\n';
	Vector2 c1 = nearest_grid_input((Vector2){x + 108,y + 12});
	Vector2 c2 = nearest_grid_input((Vector2){x + 108,y + 24});
	Vector2 c3 = nearest_grid_input((Vector2){x + 108,y + 36});
	Vector2 c4 = nearest_grid_input((Vector2){x + 108,y + 48});
	Vector2 c5 = nearest_grid_input((Vector2){x + 108,y + 60});
	Connection_points connections = dmm.connections;
	connections.c1_x = c1.x;
	connections.c1_y = c1.y;
	connections.c2_x = c2.x;
	connections.c2_y = c2.y;
	connections.c3_x = c3.x;
	connections.c3_y = c3.y;
	connections.c4_x = c4.x;
	connections.c4_y = c4.y;
	connections.c5_x = c5.x;
	connections.c5_y = c5.y;
	dmm.connections = connections;
}

void update_connection_connections(std::vector<Connection> & connection){
	for (Connection & segment : connection){
		int x1 = segment.x_min;
		int y1 = segment.y_min;
		int x2 = segment.x_max;
		int y2 = segment.y_max;
		segment.connections = {
			x1,
			y1,
			x2,
			y2,
			0,
			0,
			0,
			0,
			0,
			0
		};
	}
}

void get_input(Camera2D & camera){	
	mouse_was_clicked = mouse_is_clicked;
	mouse_is_clicked = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	Vector2 mouseWorldPos = nearest_grid_point();
	// following block is for key-based commands
	int key = GetKeyPressed();
	switch(key){
		case KEY_H:
			highlight_interactables = !highlight_interactables;
			break;
		case KEY_G:
			highlight_connections = !highlight_connections;
			break;
		case KEY_C:
			connect_mode = !connect_mode;
			break;
	}

	connect();
	if (is_cursor_in_x(regions.at("world"), mouse_pos)){
		if (resistor_being_added &&
			!mouse_is_clicked &&
			mouse_was_clicked){
			Vector2 nearest = nearest_grid_point();
			resistors.push_back({
				std::string("R") + std::to_string(resistors.size() + 1), 
				(int)nearest.x,
				(int)nearest.y,
				EAST,
				1000,
				default_connection
			});
			resistor_being_added = false;
			update_resistor_connections(resistors[resistors.size() - 1]);
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
			update_source_connections(sources[sources.size() - 1]);
			world_buttons.push_back({
                                sources[sources.size() - 1].name,
                                sources[sources.size() - 1].x - 30,
                                sources[sources.size() - 1].x + 41,
                                sources[sources.size() - 1].y,
                                sources[sources.size() - 1].y + 60,
                                false
                        });
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
			update_dmm_connections(DMMs[DMMs.size() - 1]);
			world_buttons.push_back({
                                DMMs[DMMs.size() - 1].name,
                                DMMs[DMMs.size() - 1].x,
                                DMMs[DMMs.size() - 1].x + 108,
                                DMMs[DMMs.size() - 1].y,
                                DMMs[DMMs.size() - 1].y + 72,
                                false
                        });
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

	else if(is_cursor_on_button(buttons[0], mouse_pos)){ // new source
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
	else if(is_cursor_on_button(buttons[1], mouse_pos)){ // new resistor
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
	else if(is_cursor_on_button(buttons[2], mouse_pos)){ // new agilent DMM
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

Vector2 nearest_grid_point(){
	Vector2 result;
	result.x = 12 * (int)((mouse_world_pos.x + 6) / 12);
	result.y = 12 * (int)((mouse_world_pos.y + 6) / 12);
	return result;
}

void connect(){
	if (!connect_mode){
		tracing_connection = 0;
		return;
	} 
	else if (!mouse_is_clicked &&
		mouse_was_clicked &&
		!tracing_connection){
		Vector2 nearest = nearest_grid_point();
		connections.push_back({{
			(int)nearest.x,
			(int)nearest.x,
			(int)nearest.y,
			(int)nearest.y,
			0,
			default_connection	
		}});
		update_connection_connections(connections[connections.size() - 1]);
		tracing_connection = true;
	}
	else if(tracing_connection &&
		!mouse_is_clicked){
		larger_connection();
	}
	else if(tracing_connection &&
		mouse_is_clicked &&
		!mouse_was_clicked){
		tracing_connection = false;
		Connection conn = connections[connections.size() - 1][connections[connections.size() - 1].size() - 1];
		if (!is_point_valid_node_connect(conn.x_max,conn.y_max)){
			tracing_connection = true;
			connections[connections.size() - 1].push_back({
				conn.x_max,
				conn.x_max,
				conn.y_max,
				conn.y_max,
				0,
				default_connection		
			});
		}
		else {
			connect_mode = false;
		}
		update_connection_connections(connections[connections.size() - 1]);
	}
}

void larger_connection(){
	Vector2 nearest = nearest_grid_point();
	Connection & connection = connections[connections.size() - 1][connections[connections.size() - 1].size() - 1];
	if (abs(mouse_world_pos.x - connection.x_min) > 
		abs(mouse_world_pos.y - connection.y_min)){
		connection.x_max = nearest.x;
		connection.y_max = connection.y_min;
	}
	else {
		connection.x_max = connection.x_min;
		connection.y_max = nearest.y;
	}
	return;
}

void highlight_connection(){
	for (auto element : resistors){
		DrawCircle(
			element.connections.c1_x,
			element.connections.c1_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c2_x,
			element.connections.c2_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c3_x,
			element.connections.c3_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c4_x,
			element.connections.c4_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c5_x,
			element.connections.c5_y,
			5,
			(Color){255, 0, 0, 100}
		);
	}
	for (auto element : sources){
		DrawCircle(
			element.connections.c1_x,
			element.connections.c1_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c2_x,
			element.connections.c2_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c3_x,
			element.connections.c3_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c4_x,
			element.connections.c4_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c5_x,
			element.connections.c5_y,
			5,
			(Color){255, 0, 0, 100}
		);
	}
	for (auto element : DMMs){
		DrawCircle(
			element.connections.c1_x,
			element.connections.c1_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c2_x,
			element.connections.c2_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c3_x,
			element.connections.c3_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c4_x,
			element.connections.c4_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c5_x,
			element.connections.c5_y,
			5,
			(Color){255, 0, 0, 100}
		);			
	}
	for (auto connection : connections){
		for(auto element : connection){
		DrawCircle(
			element.connections.c1_x,
			element.connections.c1_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c2_x,
			element.connections.c2_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c3_x,
			element.connections.c3_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c4_x,
			element.connections.c4_y,
			5,
			(Color){255, 0, 0, 100}
		);
		DrawCircle(
			element.connections.c5_x,
			element.connections.c5_y,
			5,
			(Color){255, 0, 0, 100}
		);			
	}
	}
}

Vector2 nearest_grid_input(Vector2 position){
	Vector2 result;
	result.x = 12 * (int)((position.x + 6) / 12);
	result.y = 12 * (int)((position.y + 6) / 12);
	return result;
}
