#include "multiple_simulator.hpp"

int height = 500;
int width = 1000;
std::vector<>

int main(){
	// Loading resources
	Image mult_front = LoadImage("resources/multisim_main.png");
	Texture2D mult_texture = LoadTextureFromImage(mult_front);
	UnloadImage(mult_front);

	Image agilent_front = LoadImage("resources/agilent_front.png");
	Texture2D agilent_texture = LoadTextureFromImage(agilent_front);
	UnloadImage(agilent_front);

	Image agilent_component = LoadImage("resources/agilent_component.png");
	Texture2D agilent_component_texture = LoadTextureFromImage(agilent_component);
	UnloadImage(agilent_component);

	Image background = LoadImage("resources/multisim_background.png");
	Texture2D background_texture = LoadTextureFromImage(background);
	UnloadImage(background);

	Image resistor = LoadImage("resources/resistor.png");
	Texture2D resistor_texture = LoadTextureFromImage(resistor);
	UnloadImage(resistor);

	Image source = LoadImage("resources/source.png");
	Texture2D source_texture = LoadTextureFromImage(source);
	UnloadImage(source);

	// Getting dimensions for window
	int current_monitor = GetCurrentMonitor();
	height = GetMonitorHeight(current_monitor);
	width = GetMonitorWidth(current_monitor);

	// initializing window
	InitWindow(width, height, "Multiple Simulator");
	SetTargetFPS(60);

	// camera shenanigans
	Camera2D camera = {0};
	camera.zoom = 1.0f;

	while(!WindowShouldClose()){
		get_input(camera);
	}

	UnloadTexture(mult_texture);
	UnloadTexture(agilent_texture);
	UnloadTexture(agilent_component_texture);
	UnloadTexture(background_texture);
	UnloadTexture(resistor_texture);
	UnloadTexture(source_texture);
	return 0;

}
