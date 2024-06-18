#include "renderer.h"

U32 white = 0xffffff;
U32 red = 0xff0000;

Renderer::Renderer(int width, int height) {
  std::cout << "Creating Window\n";

	bool running = true;
	while (running) {
		//Draw
		rWindow->ClearScreen(0x000000);
    DrawLine(13, 20, 80, 40, 0xffffff);
    DrawLine(20, 13, 40, 80, red); 
    DrawLine(80, 40, 13, 20, red);

		if (!rWindow->ProcessMessages()) {
			std::cout << "Closing Window\n";
			running = false;
		}
	}

	delete rWindow;
}


void Renderer::DrawLine(int x0, int y0, int x1, int y1, U32 color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    for (int x=x0; x<=x1; x++) { 
        float t = (x-x0)/(float)(x1-x0); 
        int y = y0*(1.-t) + y1*t; 
        rWindow->DrawPixel(x, y, color);
    } 
}