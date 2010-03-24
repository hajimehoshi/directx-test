#ifndef DEVICE_H
#define DEVICE_H

#include <tchar.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

namespace DXTest {

  class Device {
  public:
    Device(HWND hwnd);
    ~Device() throw();
    void Render();
  private:
    IDirect3D9* Direct3D;
    IDirect3DDevice9* Direct3DDevice;
    IDirect3DVertexBuffer9* Direct3DVertexBuffer;
    IDirect3DTexture9* Direct3DTexture;
  };

}

#endif
