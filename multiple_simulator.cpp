#include "multiple_simulator.hpp"

int main(){

        // Getting dimensions for window
        int current_monitor = GetCurrentMonitor();
        height = GetMonitorHeight(current_monitor);
        width = GetMonitorWidth(current_monitor);

        // initializing window
        InitWindow(width, height, "Multiple Simulator");
	height = GetScreenHeight();
	width = GetScreenWidth();

        SetTargetFPS(60);

	Camera2D camera = {0};
        camera.zoom = 1.0f;
	
	// Loading resources;
	// Multiple simulator front
	Font arial = LoadFont("resources/arial.ttf");
	Image mult_front = LoadImage("resources/multisim_main.png");
	float ratio_x = (float)width / (float)mult_front.width;
	float ratio_y = (float)height / (float)mult_front.height;
	// Auto scaling button locations so they fit the resize
	for(auto & pair : regions){
		pair.second.x_min *= ratio_x;
		pair.second.x_max *= ratio_x;
		pair.second.y_min *= ratio_y;
		pair.second.y_max *= ratio_y;
	}
	for(auto & button : buttons){
		button.x_min *= ratio_x;
		button.x_max *= ratio_x;
		button.y_min *= ratio_y;
		button.y_max *= ratio_y;
	}
	ImageResize(&mult_front, width, height);
	Texture2D mult_texture = LoadTextureFromImage(mult_front);
	UnloadImage(mult_front);
	
	// Agilent DMM ui
	Image agilent_front = LoadImage("resources/agilent_front.png");
	Texture2D agilent_texture = LoadTextureFromImage(agilent_front);
	UnloadImage(agilent_front);
	
	// Agilent component
	Image agilent_component = LoadImage("resources/agilent_component.png");
	Texture2D agilent_component_texture = LoadTextureFromImage(agilent_component);
	UnloadImage(agilent_component);

	// Multiple Simulator backround (world)
	Image background = LoadImage("resources/multisim_background.png");
	Texture2D background_texture = LoadTextureFromImage(background);
	UnloadImage(background);

	// Resistor component sprite
	Image resistor = LoadImage("resources/resistor.png");
	Texture2D resistor_texture = LoadTextureFromImage(resistor);
	UnloadImage(resistor);

	// Power Source sprite
	Image source = LoadImage("resources/source.png");
	Texture2D source_texture = LoadTextureFromImage(source);
	UnloadImage(source);

	// Creating vectors that will contain world component information

	while(!WindowShouldClose()){
		get_input(camera);
		mouse_pos = GetMousePosition();
		BeginDrawing();

		BeginMode2D(camera);
                ClearBackground(GRAY);
		mouse_world_pos = GetScreenToWorld2D(GetMousePosition(), camera);
                DrawTexture(background_texture, 0, 0, WHITE);
		for(auto resistor : resistors){
			DrawTexture(resistor_texture, resistor.x, resistor.y, WHITE);
			DrawTextEx(arial, resistor.name.c_str(), (Vector2){(float)resistor.x + 22, (float)resistor.y - 20}, 15.0, 2, BLACK);
		}
		for(auto source : sources){
			DrawTexture(source_texture, source.x, source.y, WHITE);
			DrawTextEx(arial, source.name.c_str(), (Vector2){(float)source.x - 25, (float)source.y + 16}, 15.0, 2, BLACK);
		}
		for(auto dmm : DMMs){
			DrawTexture(agilent_component_texture, dmm.x, dmm.y, WHITE);
		}
		for(auto node : nodes){
			DrawCircle(node.x, node.y, 5, RED);
		}
		for(auto connection : connections){
			for(auto sub_con : connection)
				DrawLineEx(
					(Vector2){
					(float)sub_con.x_min, 
					(float)sub_con.y_min},
					(Vector2){
					(float)sub_con.x_max, 
					(float)sub_con.y_max}, 
					1.5, 
					RED
				);
			}
		if (highlight_interactables){
                        for (auto button : world_buttons){
                                DrawRectangle(
                                        button.x_min,
                                        button.y_min,
                                        button.x_max - button.x_min,
                                        button.y_max - button.y_min,
                                        {
                                                (unsigned char)255,
                                                (unsigned char)255,
                                                (unsigned char)0,
                                                (unsigned char)100
                                });
                        }
                }
		if (highlight_connections){
			highlight_connection();
		}

                EndMode2D();

		DrawTexture(mult_texture, 0, 0, WHITE);
		if (highlight_interactables){
			for (auto button : buttons){
				DrawRectangle(
					button.x_min,
					button.y_min,
					button.x_max - button.x_min,
					button.y_max - button.y_min,
					{
						(unsigned char)255,
						(unsigned char)255,
						(unsigned char)0,
						(unsigned char)100			
				});
			}
		}
		if (region_should_darken.value){
			DrawRectangle(region_should_darken.x_min,
				region_should_darken.y_min,
				region_should_darken.y_max - 
				region_should_darken.y_min,
				region_should_darken.x_max -
				region_should_darken.x_min,
				{
					(unsigned char)0,
					(unsigned char)0,
					(unsigned char)0,
					(unsigned char)50
				});
		region_should_darken.value = false;
		}
		if (resistor_being_added) DrawTexture(
			resistor_texture,
			mouse_pos.x,
			mouse_pos.y,
			WHITE	
		);
		else if (source_being_added) DrawTexture(
			source_texture,
			mouse_pos.x,
			mouse_pos.y,
			WHITE	
		);
		else if (agilent_being_added) DrawTexture(
			agilent_component_texture,
			mouse_pos.x,
			mouse_pos.y,
			WHITE		
		);
		if (connect_mode) DrawCircle(mouse_pos.x, mouse_pos.y, 5, RED);
		EndDrawing();
	}

	UnloadTexture(mult_texture);
	UnloadTexture(agilent_texture);
	UnloadTexture(agilent_component_texture);
	UnloadTexture(background_texture);
	UnloadTexture(resistor_texture);
	UnloadTexture(source_texture);
	return 0;

}
