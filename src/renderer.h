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

class Renderer{
  Window* rWindow = new Window();
  Model *model; 

  public:
    Renderer(int width, int height);

    string CompToHex(int n);
    U32 RgbToHex(int r, int g, int b);
    Vec3f Barycentric(Vec2i *pts, Vec2i P);
    void DrawTriangle(Vec2i *pts, U32 color);
    void DrawLine(Vec2i v0, Vec2i v1, U32 color);
    int IntMax(int a, int b);
    int IntMin(int a, int b);
};