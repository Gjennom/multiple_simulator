#include "multiple_simulator.hpp"

int height = 500;
int width = 1000;

Region_should_darken region_should_darken {
	0,
	0,
	0,
	0,
	0
};

std::unordered_map<std::string, Region> Regions = {
        // {x_top, x_bottom, y_top, y_bottom}
        {"world", {200, 1321, 100, 500}},
        {"place_component_prompt",{126, 160, 26, 46}},
        {"run_selector", {476, 548, 78, 100}},
        {"agilent_selector", {1340, 1362, 430, 452}},
        {"new_source", {46, 76, 216, 245}},
        {"new_resistor", {81, 111, 216, 245}}
};

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
	// Multiple simulator front UI
	Image mult_front = LoadImage("resources/multisim_main.png");
	float ratio_x = (float)width / (float)mult_front.width;
	float ratio_y = (float)height / (float)mult_front.height;
	// Auto scaling button locations so they fit the resize
	for(auto & pair : Regions){
		pair.second.x_top *= ratio_x;
		pair.second.x_bottom *= ratio_x;
		pair.second.y_top *= ratio_y;
		pair.second.y_bottom *= ratio_y;
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
	std::vector<Resistor> resistors;
	std::vector<Source> sources;
	std::vector<Node> nodes;
	std::vector<Connection> connections;

	resistors.push_back({100,500,EAST,1});
	sources.push_back({100,600,WEST,1});
	nodes.push_back({100,700,1});
	connections.push_back({50,100,500,100});
	

	while(!WindowShouldClose()){
		get_input(camera, Regions, region_should_darken);
		BeginDrawing();

		BeginMode2D(camera);
                ClearBackground(GRAY);
                DrawTexture(background_texture, 0, 0, WHITE);
		for(auto resistor : resistors){
			DrawTexture(resistor_texture, resistor.x, resistor.y, WHITE);
		}
		for(auto source : sources){
			DrawTexture(source_texture, source.x, source.y, WHITE);
		}
		for(auto node : nodes){
			DrawCircle(node.x, node.y, 5, RED);
		}
		for(auto connection : connections){
			DrawLineEx(
				(Vector2){
				(float)connection.start_x, 
				(float)connection.start_y},
				(Vector2){
				(float)connection.end_x, 
				(float)connection.end_y}, 
				1.5, 
				RED
			);
		}

                EndMode2D();

		DrawTexture(mult_texture, 0, 0, WHITE);
		if (region_should_darken.value){
			DrawRectangle(region_should_darken.x_top,
				region_should_darken.y_top,
				region_should_darken.y_bottom - 
				region_should_darken.y_top,
				region_should_darken.x_bottom -
				region_should_darken.x_top,
				{
					(unsigned char)0,
					(unsigned char)0,
					(unsigned char)0,
					(unsigned char)50
				});
		}
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
