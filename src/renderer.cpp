//Code used mostly from https://github.com/ssloy/tinyrenderer/wiki tinyrenderer lessons.

#include "renderer.h"

Model *model = NULL;
U32 white = 0xffffff;
U32 red = 0xff0000;
U32 green = 0x00ff00;
U32 blue = 0x0000ff;

int clientWidth;
int clientHeight;

Renderer::Renderer(int width, int height) {
  clientWidth = width;
  clientHeight = height;

  std::cout << "Creating Window\n";

	bool running = true;
	while (running) {

    model = new Model("../obj/african_head.obj");

		//Draw
		rWindow->ClearScreen(0x000000);

    //Wire Mesh
      // for (int i=0; i<model->nfaces(); i++) { 
      //   std::vector<int> face = model->face(i); 
      //   for (int j=0; j<3; j++) { 
      //       Vec3f v0 = model->vert(face[j]); 
      //       Vec3f v1 = model->vert(face[(j+1)%3]); 
      //       int y0 = (v0.x+1.)*width/2.; 
      //       int x0 = (v0.y+1.)*height/2.; 
      //       int y1 = (v1.x+1.)*width/2.; 
      //       int x1 = (v1.y+1.)*height/2.; 
      //       DrawLine(Vec2i(x0*1.8, y0*1.8), Vec2i(x1*1.8, y1*1.8), green); 
      //       } 
      // }

    //Rasterization 
    for (int i=0; i<model->nfaces(); i++) { 
      std::vector<int> face = model->face(i); 
      Vec2i screen_coords[3]; 
      for (int j=0; j<3; j++) { 
        Vec3f world_coords = model->vert(face[j]); 
        screen_coords[j] = Vec2i((world_coords.x+1.)*width/2., (world_coords.y+1.)*height/2.); 
      } 
      DrawTriangle(screen_coords, RgbToHex(rand()%255, rand()%255, rand()%255)); 
  }
    
		if (!rWindow->ProcessMessages()) {
			std::cout << "Closing Window\n";
			running = false;
		}
	}

  delete model;
	delete rWindow;
}

string Renderer::CompToHex(int n) {
  char hex[2];
  int i = 0;
    while (n != 0) {
        int temp = 0;
 
        temp = n % 16;
 
        if (temp < 10) {
            hex[i] = temp + 48;
            i++;
        }
        else {
            hex[i] = temp + 55;
            i++;
        }
 
        n = n / 16;
    }
 
    string hexCode = "";
    if (i == 2) {
        hexCode.push_back(hex[0]);
        hexCode.push_back(hex[1]);
    }
    else if (i == 1) {
        hexCode = "0";
        hexCode.push_back(hex[0]);
    }
    else if (i == 0)
        hexCode = "00";
 
    return hexCode;
}

U32 Renderer::RgbToHex(int r, int g, int b) {
    U32 res = 0x000000;
    std::stringstream ss;
    ss << "0x" << CompToHex(r) << CompToHex(g) << CompToHex(b);
    ss >> std::hex >> res;
    return res;
}

Vec3f Renderer::Barycentric(Vec2i *pts, Vec2i P) { 
    Vec3f u = Vec3f(pts[2].x-pts[0].x, pts[1].x-pts[0].x, pts[0].x-P.x)^Vec3f(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y-P.y);
    if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);
    return Vec3f(1.0f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
} 

void Renderer::DrawTriangle(Vec2i *pts, U32 color) {
    Vec2i bboxmin(clientWidth-1,  clientHeight-1); 
    Vec2i bboxmax(0, 0); 
    Vec2i clamp(clientWidth-1, clientHeight-1); 
    for (int i = 0; i < 3; i++) { 
        bboxmin.x = IntMax(0, IntMin(bboxmin.x, pts[i].x));
	      bboxmin.y = IntMax(0, IntMin(bboxmin.y, pts[i].y));

	      bboxmax.x = IntMin(clamp.x, IntMax(bboxmax.x, pts[i].x));
	      bboxmax.y = IntMin(clamp.y, IntMax(bboxmax.y, pts[i].y));
    } 
    
    Vec2i P; 
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) { 
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) { 
            Vec3f bc_screen  = Barycentric(pts, P); 
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue; 
            rWindow->DrawPixel(P.x, P.y, color);
        } 
    }  
}

void Renderer::DrawLine(Vec2i v0, Vec2i v1, U32 color) { 
    bool steep = false; 
    if (std::abs(v0.x-v1.x)<std::abs(v0.y-v1.y)) { 
        std::swap(v0.x, v0.y); 
        std::swap(v1.x, v1.y); 
        steep = true; 
    } 
    if (v0.x>v1.x) { 
        std::swap(v0.x, v1.x); 
        std::swap(v0.y, v1.y); 
    } 
    int dx = v1.x-v0.x; 
    int dy = v1.y-v0.y; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = v0.y; 
    for (int x=v0.x; x<=v1.x; x++) { 
        if (steep) { 
            rWindow->DrawPixel(x, y, color);
        } else { 
            rWindow->DrawPixel(y, x, color);
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (v1.y>v0.y?1:-1); 
            error2 -= dx*2; 
        } 
    } 
}

int Renderer::IntMax(int a, int b) {
 return (((a) > (b)) ? (a) : (b));
}

int Renderer::IntMin(int a, int b) {
 return (((a) < (b)) ? (a) : (b));
}