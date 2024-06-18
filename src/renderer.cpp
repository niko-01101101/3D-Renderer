//Code used mostly from https://github.com/ssloy/tinyrenderer/wiki tinyrenderer lessons.

#include "renderer.h"

Model *model = NULL;
U32 white = 0xffffff;
U32 red = 0xff0000;

Renderer::Renderer(int width, int height) {
  std::cout << "Creating Window\n";

	bool running = true;
	while (running) {

    model = new Model("../obj/african_head.obj");

		//Draw
		rWindow->ClearScreen(0x000000);

    //Wire Mesh
      for (int i=0; i<model->nfaces(); i++) { 
        std::vector<int> face = model->face(i); 
        for (int j=0; j<3; j++) { 
            Vec3f v0 = model->vert(face[j]); 
            Vec3f v1 = model->vert(face[(j+1)%3]); 
            int y0 = (v0.x+1.)*width/2.; 
            int x0 = (v0.y+1.)*height/2.; 
            int y1 = (v1.x+1.)*width/2.; 
            int x1 = (v1.y+1.)*height/2.; 
            DrawLine(x0*1.8, y0*1.8, x1*1.8, y1*1.8, white); 
            } 
      }
    

		if (!rWindow->ProcessMessages()) {
			std::cout << "Closing Window\n";
			running = false;
		}
	}

  delete model;
	delete rWindow;
}


void Renderer::DrawLine(int x0, int y0, int x1, int y1, U32 color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            rWindow->DrawPixel(x, y, color);
        } else { 
            rWindow->DrawPixel(y, x, color);
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
}