#include <iostream>
#include "raylib.h"

int main(){
	InitWindow(1000,500,"nothing to see here");

	Image background = LoadImage("resources/multisim_background.png");
	
	int height = background.height;
	int width = background.width;

	for(int i = 0; i < height / 12; i++){
		for(int j = 0; j < width / 12; j++){
			ImageDrawPixel(&background, j * 12, i * 12, GRAY);
		}
	}

	ExportImage(background, "resources/new_background.png");
	UnloadImage(background);
	CloseWindow();
	return 0;
}
