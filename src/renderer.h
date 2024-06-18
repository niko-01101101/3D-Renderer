#define U32 unsigned __int32

#include <stdint.h>
#include <iostream>
#include "window.h"

class Renderer{
  Window* rWindow = new Window();
  
  public:
    Renderer(int width, int height);

    void DrawLine(int x0, int y0, int x1, int y1, U32 color);
};