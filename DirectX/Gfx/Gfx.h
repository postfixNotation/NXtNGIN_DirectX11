#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include <d3d11.h>
#include <d3dcompiler.h>
#include <atlbase.h>

#include <vector>
#include <fstream>

#include "Vertex.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Gfx
{
private:
    CComPtr<ID3D11Device>               m_pDevice;
    CComPtr<ID3D11DeviceContext>        m_pDeviceContext;
    CComPtr<IDXGISwapChain>             m_pSwapChain;
    CComPtr<ID3D11RenderTargetView>     m_pRenderTargetView;
    CComPtr<ID3D11VertexShader>         m_pVertexShader;
    CComPtr<ID3D11PixelShader>          m_pPixelShader;
    CComPtr<ID3D11Buffer>               m_pVBuffer;
    CComPtr<ID3D11InputLayout>          m_pInputLayout;
public:
    VOID Draw()
    {
        static DirectX::XMFLOAT4 color(0.6f, 0.8f, 1.0f, 1.0f);
        m_pDeviceContext->ClearRenderTargetView(
            m_pRenderTargetView,
            &color.x);

        UINT uVertexSize = sizeof(VERTEX);
        UINT uOffset = 0;

        m_pDeviceContext->IASetVertexBuffers(
            0,
            1,
            &m_pVBuffer.p,
            &uVertexSize,
            &uOffset);

        m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_pDeviceContext->Draw(6, 0);
        m_pSwapChain->Present(0, 0);
    }

    VOID LoadVertices()
    {
        std::vector<VERTEX> vertices = {
            { DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
            { DirectX::XMFLOAT3(0.5f, -0.5, 0.0f), DirectX::XMFLOAT3(1.0f, .0f, 0.0f) },
            { DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },

            { DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
            { DirectX::XMFLOAT3(0.5f, 0.5, 0.0f), DirectX::XMFLOAT3(1.0f, .0f, 0.0f) },
            { DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
        };

        D3D11_BUFFER_DESC buffer_desc;

        ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));

        buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        buffer_desc.ByteWidth = static_cast<UINT>(vertices.size()) * sizeof(VERTEX);
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        m_pDevice->CreateBuffer(
            &buffer_desc,
            NULL,
            &m_pVBuffer);

        D3D11_MAPPED_SUBRESOURCE mapped_subres;

        m_pDeviceContext->Map(
            m_pVBuffer,
            NULL,
            D3D11_MAP_WRITE_DISCARD,
            NULL,
            &mapped_subres);

        memcpy(
            mapped_subres.pData,
            vertices.data(),
            vertices.size() * sizeof(VERTEX));

        m_pDeviceContext->Unmap(
            m_pVBuffer,
            NULL);
    }

    VOID InitShaders()
    {
        std::fstream fstrVertex, fstrPixel;
        size_t bytesRead;
        char *buffer;

        fstrVertex.open("../bin/Debug-x64/DirectX/VertexShader.cso", std::fstream::in | std::fstream::binary);
        fstrPixel.open("../bin/Debug-x64/DirectX/PixelShader.cso", std::fstream::in | std::fstream::binary);

        fstrVertex.seekg(0, fstrVertex.end);
        bytesRead = fstrVertex.tellg();
        fstrVertex.seekg(0, fstrVertex.beg);

        buffer = new char[bytesRead];
        fstrVertex.read(buffer, bytesRead);

        m_pDevice->CreateVertexShader(
            buffer,
            bytesRead,
            nullptr,
            &m_pVertexShader
        );

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        m_pDevice->CreateInputLayout(
            inputElementDesc.data(),
            static_cast<UINT>(inputElementDesc.size()),
            buffer,
            bytesRead,
            &m_pInputLayout);

        delete[] buffer;

        m_pDeviceContext->IASetInputLayout(m_pInputLayout);

        fstrVertex.close();

        fstrPixel.seekg(0, fstrPixel.end);
        bytesRead = fstrPixel.tellg();
        fstrPixel.seekg(0, fstrPixel.beg);

        buffer = new char[bytesRead];
        fstrPixel.read(buffer, bytesRead);
        fstrPixel.close();

        m_pDevice->CreatePixelShader(
            buffer,
            bytesRead,
            NULL,
            &m_pPixelShader);

        delete[] buffer;

        m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
        m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

        return;
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
        //m_pSwapChain->SetFullscreenState(TRUE, NULL);
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
            scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

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
        m_pSwapChain->SetFullscreenState(FALSE, NULL);
        CoUninitialize();
    }
};
