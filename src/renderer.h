#define U32 unsigned __int32

using namespace std;

#include <cstdlib>
#include <stdint.h>
#include <iostream>
#include "window.h"
#include <cmath>
#include "model.h"
#include "geometry.h"
#include <string>
#include <cstdint>
#include <sstream>
#include <limits>

class Renderer{
  Window* rWindow = new Window();
  Model *model; 

  public:
    Renderer(int width, int height);

    string CompToHex(int n);
    Vec3f WorldToScreen(Vec3f v);
    U32 RgbToHex(int r, int g, int b);
    Vec3f Barycentric(Vec3f *pts, Vec3f P);
    void DrawTriangle(Vec3i t0, Vec3i t1, Vec3i t2, Vec2i uv0, Vec2i uv1, Vec2i uv2, float intensity, int *zbuffer);
    void DrawLine(Vec2i v0, Vec2i v1, U32 color);
    int IntMax(int a, int b);
    int IntMin(int a, int b);
};