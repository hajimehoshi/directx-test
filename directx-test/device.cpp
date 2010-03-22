#include "device.h"

#include <stdexcept>

namespace DXTest {

  struct CustomVertex {
    float x, y, z;
    DWORD color;
  };

  static const int D3DFVF_CUSTOMVERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE;

  Device::Device(HWND hWnd)
    : Direct3D(0),
      Direct3DDevice(0),
      Direct3DVertexBuffer(0) {
    try {
      if (!(this->Direct3D = Direct3DCreate9(D3D_SDK_VERSION))) {
        throw std::exception();
      }
      D3DPRESENT_PARAMETERS presentParameters;
      ZeroMemory(&presentParameters, sizeof(presentParameters));
      presentParameters.SwapEffect             = D3DSWAPEFFECT_DISCARD;
      presentParameters.Windowed               = TRUE;
      presentParameters.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
      if (FAILED(this->Direct3D->CreateDevice(D3DADAPTER_DEFAULT,
                                              D3DDEVTYPE_HAL,
                                              hWnd,
                                              D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                              &presentParameters,
                                              &this->Direct3DDevice))) {
        throw std::exception();
      }
      this->Direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
      this->Direct3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
      if (FAILED(this->Direct3DDevice->CreateVertexBuffer(3 * sizeof(CustomVertex),
                                                          0,
                                                          D3DFVF_CUSTOMVERTEX,
                                                          D3DPOOL_DEFAULT,
                                                          &this->Direct3DVertexBuffer,
                                                          0))) {
        throw std::exception();
      }
      CustomVertex Vertices[] = {
        {  0.0f,  1.0f, 1.0f, 0xffff0000, },
        { -1.0f, -1.0f, 1.0f, 0xff00ff00, },
        {  1.0f, -1.0f, 1.0f, 0xff0000ff, },
      };
      void* pVertices;
      if (FAILED(this->Direct3DVertexBuffer->Lock(0, sizeof(Vertices), (void**)&pVertices, 0))) {
        throw std::exception();
      }
      memcpy(pVertices, Vertices, sizeof(Vertices));
      this->Direct3DVertexBuffer->Unlock();
    } catch (...) {
      this->~Device();
      throw;
    }
  }

  Device::~Device() throw() {
    if (this->Direct3DVertexBuffer) {
      this->Direct3DVertexBuffer->Release();
      this->Direct3DVertexBuffer = 0;
    }
    if (this->Direct3DDevice) {
      this->Direct3DDevice->Release();
      this->Direct3DDevice = 0;
    }
    if (this->Direct3D) {
      this->Direct3D->Release();
      this->Direct3D = 0;
    }
  }

  void Device::Render() {
    this->Direct3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    if (SUCCEEDED(this->Direct3DDevice->BeginScene())) {
      {
        {
          /*D3DXMATRIXA16 matWorld;
          UINT iTime = timeGetTime() % 1000;
          float fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
          D3DXMatrixRotationY(&matWorld, fAngle);
          D3DXMatrixIdentity(&matWorld);
          this->Direct3DDevice->SetTransform(D3DTS_WORLD, &matWorld);*/
        }
        {
          D3DXVECTOR3 vEvePt(0.0f, 0.0f, 0.0f);
          D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 1.0f);
          D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
          D3DXMATRIXA16 matView;
          D3DXMatrixLookAtLH(&matView,
                             &vEvePt,
                             &vLookatPt,
                             &vUpVec);
          this->Direct3DDevice->SetTransform(D3DTS_VIEW, &matView);
        }
        {
          D3DXMATRIXA16 matProj;
          D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 3, 0.75f, 0.0f, 1000.0f);
          this->Direct3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
        }
        {
          /*D3DVIEWPORT9 viewport;
          viewport.X = 0;
          viewport.Y = 0;
          viewport.Width = 640;
          viewport.Height = 480;*/
        }
      }
      this->Direct3DDevice->SetStreamSource(0, this->Direct3DVertexBuffer, 0, sizeof(CustomVertex));
      this->Direct3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
      this->Direct3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
      this->Direct3DDevice->EndScene();
    }
    this->Direct3DDevice->Present(0, 0, 0, 0);
  }

}