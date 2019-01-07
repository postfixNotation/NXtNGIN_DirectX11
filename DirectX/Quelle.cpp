#include "Window/MainWindow.h"
#include "Gfx/Gfx.h"
#include <memory>

constexpr UINT kuiWidth = 1920, kuiHeight = 1080, kuiXpos = 0, kuiYpos = 0;

class D3D
{
private:
    CComPtr<IDXGISwapChain>         swapchain;
    CComPtr<ID3D11Device>           dev;
    CComPtr<ID3D11DeviceContext>    devcon;
    CComPtr<ID3D11RenderTargetView> backbuffer;
    CComPtr<ID3D11InputLayout>      pLayout;
    CComPtr<ID3D11VertexShader>     pVS;
    CComPtr<ID3D11PixelShader>      pPS;
    CComPtr<ID3D11Buffer>           pVBuffer;
    CComPtr<ID3DBlob>               m_pBlobVS, m_pBlobPS;
public:
    void InitD3D(HWND hWnd)
    {
        HRESULT hr = CoInitializeEx(
            NULL,
            COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

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

        scd.BufferDesc.Width = kuiWidth;
        scd.BufferDesc.Height = kuiHeight;
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
            &swapchain,
            &dev,
            &pFeatureLevels,
            &devcon);

        // get the address of the back buffer
        CComPtr<ID3D11Texture2D> pBackBuffer;
        swapchain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

        // use the back buffer address to create the render target
        dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);

        // set the render target as the back buffer
        devcon->OMSetRenderTargets(1, &backbuffer.p, NULL);

        // Set the viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = kuiWidth;
        viewport.Height = kuiHeight;

        devcon->RSSetViewports(1, &viewport);

        InitPipeline();
        InitGraphics();
    }

    void RenderFrame(void)
    {
        const float color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
        devcon->ClearRenderTargetView(backbuffer, color);

        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        devcon->IASetVertexBuffers(
            0,
            1,
            &pVBuffer.p,
            &stride,
            &offset);

        devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        devcon->Draw(3, 0);
        swapchain->Present(0, 0);
    }

    void CleanD3D(void)
    {
        swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
        CoUninitialize();
    }

    void InitGraphics()
    {
        // create a triangle using the VERTEX struct
        std::vector<VERTEX> OurVertices =
        {
            {DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)},
            {DirectX::XMFLOAT3(0.45f, -0.5, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)},
            {DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)}
        };

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(VERTEX) * static_cast<UINT>(OurVertices.size());
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        dev->CreateBuffer(&bd, NULL, &pVBuffer);

        // copy the vertices into the buffer
        D3D11_MAPPED_SUBRESOURCE ms;
        devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(
            ms.pData,
            OurVertices.data(),
            sizeof(VERTEX) * OurVertices.size());
        devcon->Unmap(pVBuffer, NULL);
    }

    void InitPipeline()
    {
        D3DCompileFromFile(
            TEXT("Shaders/VertexShader.hlsl"),
            NULL,
            NULL,
            "VShader",
            "vs_5_0",
            0,
            0,
            &m_pBlobVS,
            NULL);
        D3DCompileFromFile(
            TEXT("Shaders/PixelShader.hlsl"),
            NULL,
            NULL,
            "PShader",
            "ps_5_0",
            0,
            0,
            &m_pBlobPS,
            NULL);

        // encapsulate both shaders into shader objects
        dev->CreateVertexShader(m_pBlobVS->GetBufferPointer(), m_pBlobVS->GetBufferSize(), NULL, &pVS);
        dev->CreatePixelShader(m_pBlobPS->GetBufferPointer(), m_pBlobPS->GetBufferSize(), NULL, &pPS);

        // set the shader objects
        devcon->VSSetShader(pVS, 0, 0);
        devcon->PSSetShader(pPS, 0, 0);

        // create the input layout object
        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        dev->CreateInputLayout(ied, 2, m_pBlobVS->GetBufferPointer(), m_pBlobVS->GetBufferSize(), &pLayout);
        devcon->IASetInputLayout(pLayout);
    }
};
