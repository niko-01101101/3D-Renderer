//Code used mostly from https://github.com/ssloy/tinyrenderer/wiki tinyrenderer lessons.

#include "renderer.h"

Model *model = NULL;
int *zbuffer = NULL;
Vec3f light_dir(0,0,-1);
Vec3f camera(0,0,3);

U32 white = 0xffffff;
U32 red = 0xff0000;
U32 green = 0x00ff00;
U32 blue = 0x0000ff;

int clientWidth;
int clientHeight;
const int depth  = 255;

Vec3f m2v(Matrix m) {
    return Vec3f(m[0][0]/m[3][0], m[1][0]/m[3][0], m[2][0]/m[3][0]);
}

Matrix v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}

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

    float *zbuffer = new float[width*height];
    for (int i=width*height; i--; zbuffer[i] = -99999999999999);

    //Rasterization 
    for (int i=0; i<model->nfaces(); i++) { 
      std::vector<int> face = model->face(i);
        Vec3f pts[3];
        for (int i=0; i<3; i++) pts[i] = WorldToScreen(model->vert(face[i]));
        float intensity =  10; 
        DrawTriangle(pts, zbuffer, RgbToHex(intensity, intensity, intensity)); 
    
  }
    
		if (!rWindow->ProcessMessages()) {
			std::cout << "Closing Window\n";
			running = false;
		}
	}

  delete model;
	delete rWindow;
}

Vec3f Renderer::WorldToScreen(Vec3f v) {
    return Vec3f(int((v.x+1.)*clientWidth/2.+.5), int((v.y+1.)*clientHeight/2.+.5), v.z);
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

Vec3f Renderer::Barycentric(Vec3f *pts, Vec3f P) { 
    Vec3f u = Vec3f(pts[2].x-pts[0].x, pts[1].x-pts[0].x, pts[0].x-P.x)^Vec3f(pts[2].y-pts[0].y, pts[1].y-pts[0].y, pts[0].y-P.y);
    if (std::abs(u.z) < 1) return Vec3f(-1, 1, 1);
    return Vec3f(1.0f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z); 
} 

void Renderer::DrawTriangle(Vec3i t0, Vec3i t1, Vec3i t2, Vec2i uv0, Vec2i uv1, Vec2i uv2, float intensity, int *zbuffer) {
    if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
    if (t0.y>t1.y) { std::swap(t0, t1); std::swap(uv0, uv1); }
    if (t0.y>t2.y) { std::swap(t0, t2); std::swap(uv0, uv2); }
    if (t1.y>t2.y) { std::swap(t1, t2); std::swap(uv1, uv2); }

    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec3i A   =               t0  + Vec3f(t2-t0  )*alpha;
        Vec3i B   = second_half ? t1  + Vec3f(t2-t1  )*beta : t0  + Vec3f(t1-t0  )*beta;
        Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
        Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;
        if (A.x>B.x) { std::swap(A, B); std::swap(uvA, uvB); }
        for (int j=A.x; j<=B.x; j++) {
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            Vec3i   P = Vec3f(A) + Vec3f(B-A)*phi;
            Vec2i uvP =     uvA +   (uvB-uvA)*phi;
            int idx = P.x+P.y*width;
            if (zbuffer[idx]<P.z) {
                zbuffer[idx] = P.z;
                TGAColor color = model->diffuse(uvP);
                image.set(P.x, P.y, TGAColor(color.r*intensity, color.g*intensity, color.b*intensity));
            }
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