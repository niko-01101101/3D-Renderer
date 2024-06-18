#define U32 unsigned __int32

#include <stdint.h>
#include <iostream>
#include "window.h"
#include <vector>
#include <cmath>
#include "model.h"
#include "geometry.h"

class Renderer{
  Window* rWindow = new Window();
  Model *model; 

  public:
    Renderer(int width, int height);

    void DrawLine(int x0, int y0, int x1, int y1, U32 color);
};