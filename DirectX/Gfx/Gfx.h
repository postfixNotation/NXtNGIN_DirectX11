#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <atlbase.h>

#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dxgi.lib")

class Gfx
{
private:
    CComPtr<ID3D11Device>               m_pDevice;
    CComPtr<ID3D11DeviceContext>        m_pDeviceContext;
    CComPtr<IDXGISwapChain>             m_pSwapChain;
    CComPtr<ID3D11RenderTargetView>     m_pRenderTargetView;
public:
    VOID RenderFrame()
    {
        //const float color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        DirectX::XMFLOAT4 color(0.0f, 0.2f, 0.4f, 1.0f);
        m_pDeviceContext->ClearRenderTargetView(
            m_pRenderTargetView,
            &color.x);

        m_pSwapChain->Present(0, 0);
    }

    VOID SetViewport(FLOAT fWidth, FLOAT fHeight)
    {
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = fWidth;
        viewport.Height = fHeight;

        m_pDeviceContext->RSSetViewports(1, &viewport);
    }

    HRESULT SetRenderTarget()
    {
        CComPtr<ID3D11Texture2D> pBackBuffer;

        HRESULT hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        if (SUCCEEDED(hr))
            hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
        if (SUCCEEDED(hr))
            m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView.p, NULL);

        return hr;
    }

    BOOL InitD3D(HINSTANCE hInstance, HWND hWnd, UINT uiWidth, UINT uiWeight)
    {
        HRESULT hr = CoInitializeEx(
            NULL,
            COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        if (SUCCEEDED(hr))
        {
            constexpr D3D_FEATURE_LEVEL pFeatures[] = {
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1,
            };
            D3D_FEATURE_LEVEL pFeatureLevels;

            DXGI_SWAP_CHAIN_DESC scd;
            ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

            scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

            scd.BufferDesc.Width = uiWidth;
            scd.BufferDesc.Height = uiWeight;
            scd.BufferDesc.RefreshRate = { 60 , 1 };

            scd.BufferCount = 1;
            scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            //scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            scd.OutputWindow = hWnd;
            scd.SampleDesc.Count = 1;
            scd.SampleDesc.Quality = 0;
            scd.Windowed = TRUE;

            D3D11CreateDeviceAndSwapChain(
                NULL,
                D3D_DRIVER_TYPE_HARDWARE,
                NULL,
                D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG,
                pFeatures,
                ARRAYSIZE(pFeatures),
                D3D11_SDK_VERSION,
                &scd,
                &m_pSwapChain,
                &m_pDevice,
                &pFeatureLevels,
                &m_pDeviceContext);
        }
        else
        {
            return FALSE;
        }

        return SUCCEEDED(hr);
    }

    ~Gfx()
    {
        CoUninitialize();
    }
};
