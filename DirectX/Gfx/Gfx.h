#pragma once
#include <d3d11.h>
#include <atlbase.h>

class Gfx
{
public:
    BOOL Init(HINSTANCE hInstance, HWND hWnd, UINT width, UINT height)
    {
        HRESULT hr = CoInitializeEx(
            NULL,
            COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr))
        {
            D3D11_TEXTURE2D_DESC descDepth;
            descDepth.MipLevels = 1;
            descDepth.ArraySize = 1;
            descDepth.SampleDesc.Count = 1;
            descDepth.SampleDesc.Quality = 0;
            descDepth.Usage = D3D11_USAGE_DEFAULT;
            descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            descDepth.CPUAccessFlags = 0;
            descDepth.MiscFlags = 0;
            hr = pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

            D3D11_DEPTH_STENCIL_DESC dsDesc;

            dsDesc.DepthEnable = true;
            dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

            dsDesc.StencilEnable = true;
            dsDesc.StencilReadMask = 0xFF;
            dsDesc.StencilWriteMask = 0xFF;

            dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
            dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
            dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            D3D_FEATURE_LEVEL pFeatures[] = {
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1,
            };

            D3D_FEATURE_LEVEL pFeatureLevels;

            DXGI_SWAP_CHAIN_DESC scd;
            scd.BufferDesc.Width = width;
            scd.BufferDesc.Height = height;
            scd.BufferDesc.RefreshRate = { 60 , 1 };
            scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            scd.OutputWindow = hWnd;

            hr = D3D11CreateDeviceAndSwapChain(
                pAdapter,
                D3D_DRIVER_TYPE_HARDWARE,
                hInstance,
                D3D11_CREATE_DEVICE_SINGLETHREADED,
                pFeatures,
                sizeof(pFeatures) / sizeof(D3D_FEATURE_LEVEL),
                D3D11_SDK_VERSION,
                &scd,
                &pSwapChain,
                &pDevice,
                &pFeatureLevels,
                &pDeviceContext
            );
        }

        pDeviceContext->OMSetDepthStencilState(pDSState, 1);
        CoUninitialize();
        return SUCCEEDED(hr);
    }
private:
    CComPtr<ID3D11Device>               pDevice;
    CComPtr<IDXGIAdapter>               pAdapter;
    CComPtr<IDXGISwapChain>             pSwapChain;
    CComPtr<ID3D11Texture2D>            pDepthStencil;
    CComPtr<ID3D11DeviceContext>        pDeviceContext;
    CComPtr<ID3D11DepthStencilState>    pDSState;
    CComPtr<ID3D11DepthStencilView>     pDSV;
};