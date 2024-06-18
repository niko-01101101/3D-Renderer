#pragma once
#define U32 unsigned __int32
#include <Windows.h>
#include <stdint.h>

#pragma comment (lib, "user32.lib")
#pragma comment (lib, "gdi32.lib")

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window {
public:
  Window();
  Window(const Window&) = delete;
  Window& operator = (const Window&) = delete;
  ~Window();

  void DrawPixel(U32 x, U32 y, U32 color);

  void ClearScreen(U32 color);

  bool ProcessMessages();
private:
  HINSTANCE m_hInstance;
  HWND m_hWnd;
};