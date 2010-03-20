#include <cassert>
#include <iostream>
#include <stdexcept>

#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>

#include "device.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     TCHAR* lpCmdLine,
                     int nCmdShow) {
  try {
    class WndClass {
    public:
      WndClass(HINSTANCE hInstance) throw() 
        : HInstance(hInstance), ClassName(_T("DirectXTestWindowClass")) {
        WNDCLASSEX wc;
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = WndProc;
        wc.hInstance     = this->HInstance;
        wc.hCursor       = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName  = 0;
        wc.lpszClassName = this->GetClassName();
        RegisterClassEx(&wc);
      }
      ~WndClass() throw() {
        UnregisterClass(this->GetClassName(), this->HInstance);
      }
      const TCHAR* GetClassName() const throw() {
        return this->ClassName;
      }
    private:
      const HINSTANCE HInstance;
      const TCHAR* ClassName;
    } wndClass(hInstance);

    RECT rect = {0, 0, 640, 480};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hWnd = CreateWindow(wndClass.GetClassName(),
                             _T("Fooo"),
                             WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             rect.right - rect.left, rect.bottom - rect.top,
                             0, 0, hInstance, 0);
    if (!hWnd) {
      throw std::exception();
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    DXTest::Device device(hWnd);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
      if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      } else {
        device.Render();
      }
    }

    return (int)msg.wParam;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << _T("Error!") << std::endl;
    return EXIT_FAILURE;
  }
}

LRESULT CALLBACK WndProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}
