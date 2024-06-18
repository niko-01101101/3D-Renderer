#include "window.h"

void *memory;
U32 clientWidth;
U32 clientHeight;

BITMAPINFO bitmapInfo;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  HDC hdc = GetDC(hWnd);

  StretchDIBits(hdc,
                      0,
                      0,
                      clientWidth,
                      clientHeight,
                      0,
                      0,
                      clientWidth,
                      clientHeight,
                      memory,
                      &bitmapInfo,
                      DIB_RGB_COLORS,
                      SRCCOPY
                      );

  switch (uMsg){
    case WM_CLOSE:
      DestroyWindow(hWnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Window() : m_hInstance(GetModuleHandle(nullptr)) {
      LPCSTR CLASS_NAME = "Renderer Window";

      WNDCLASS wndClass = { };
      wndClass.lpszClassName = CLASS_NAME;
      wndClass.hInstance = m_hInstance;
      wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
      wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
      wndClass.lpfnWndProc = WindowProc;

      RegisterClass(&wndClass);

      DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

      int width = 640, height = 480;

      RECT rect;
      rect.left = 250;
      rect.top = 250;
      rect.right = rect.left + width;
      rect.bottom = rect.top + height;

      AdjustWindowRect(&rect, style, false);

      clientWidth = width;
      clientHeight = height;
    
      memory = VirtualAlloc(0, 
                          clientWidth * clientHeight * 4,
                          MEM_RESERVE|MEM_COMMIT,
                          PAGE_READWRITE
                          );

       bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader),
       bitmapInfo.bmiHeader.biWidth = clientWidth,
       bitmapInfo.bmiHeader.biHeight = clientHeight,
       bitmapInfo.bmiHeader.biPlanes = 1,
       bitmapInfo.bmiHeader.biBitCount = 32,
       bitmapInfo.bmiHeader.biCompression = BI_RGB
    ;

      m_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        (LPCSTR)"Awesome Renderer",
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hInstance,
        NULL
      );

      ShowWindow(m_hWnd, SW_SHOW);
  }


Window::~Window() {
    LPCSTR CLASS_NAME = "Renderer Window";

    UnregisterClass(CLASS_NAME, m_hInstance);
}

void Window::DrawPixel(U32 x, U32 y, U32 color) {
    U32 *pixel = (U32 *)memory;
    pixel += y * clientWidth + x;
    *pixel = color;
}

void Window::ClearScreen(U32 color) {
    U32 *pixel = (U32 *)memory;
    for(U32 i = 0; i < clientWidth * clientHeight; ++i) {
        *pixel++ = color;
    }
}

bool Window::ProcessMessages() {
    MSG msg = {};
    
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        return false;
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    return true; 
}