//#include <atlbase.h>
//#include <d3d11.h>
//#include <d3dcompiler.h>
//#include <directxmath.h>
//#include <dxgi.h>
//#include <windows.h>
//#include <comdef.h>

//// include the Direct3D Library file
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "dxgi.lib")

//CComPtr<ID3D11InputLayout> pLayout; // the pointer to the input layout
//CComPtr<ID3D11VertexShader> pVS; // the pointer to the vertex shader
//CComPtr<ID3D11PixelShader> pPS; // the pointer to the pixel shader
//CComPtr<ID3D11Buffer> pVBuffer; // the pointer to the vertex buffer
//// a struct to define a single vertex
//struct VERTEX
//{
//    FLOAT X, Y, Z;
//    DirectX::XMFLOAT3 Color;
//};

//void InitGraphics(void); // creates the shape to render
//void InitPipeline(void); // loads and prepares the shaders

//int WINAPI WinMain(HINSTANCE hInstance,
//    HINSTANCE hPrevInstance,
//    LPSTR lpCmdLine,
//    int nCmdShow)
//{
//    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
//
//    if (SUCCEEDED(hr))
//    {
//        HWND hWnd;
//        WNDCLASSEX wc;
//
//        ZeroMemory(&wc, sizeof(WNDCLASSEX));
//
//        wc.cbSize = sizeof(WNDCLASSEX);
//        wc.style = CS_HREDRAW | CS_VREDRAW;
//        wc.lpfnWndProc = WindowProc;
//        wc.hInstance = hInstance;
//        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//        wc.lpszClassName = TEXT("WindowClass");
//
//        RegisterClassEx(&wc);
//
//        RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
//        AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);
//
//        hWnd = CreateWindowEx(NULL,
//            TEXT("WindowClass"),
//            TEXT("Our First Direct3D Program"),
//            WS_OVERLAPPEDWINDOW,
//            300,
//            300,
//            wr.right - wr.left,
//            wr.bottom - wr.top,
//            NULL,
//            NULL,
//            hInstance,
//            NULL);
//
//        ShowWindow(hWnd, nCmdShow);
//
//        // set up and initialize Direct3D
//        InitD3D(hWnd);
//
//        // enter the main loop:
//        bool bGotMsg;
//        MSG msg;
//        PeekMessage(&msg, hWnd, 0U, 0U, PM_NOREMOVE);
//
//        while (WM_QUIT != msg.message)
//        {
//            bGotMsg = (PeekMessage(&msg, hWnd, 0U, 0U, PM_REMOVE) != FALSE);
//
//            if (bGotMsg)
//            {
//                TranslateMessage(&msg);
//                DispatchMessage(&msg);
//            }
//            else
//            {
//                RenderFrame();
//            }
//        }
//
//        // clean up DirectX and COM
//        CleanD3D();
//        CoUninitialize();
//    }
//    return 0;
//}

//// this function initializes and prepares Direct3D for use
//void InitD3D(HWND hWnd)
//{
//    // create a struct to hold information about the swap chain
//    DXGI_SWAP_CHAIN_DESC scd;
//
//    // clear out the struct for use
//    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
//
//    // fill the swap chain description struct
//    scd.BufferCount = 1; // one back buffer
//    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
//    scd.BufferDesc.Width = SCREEN_WIDTH; // set the back buffer width
//    scd.BufferDesc.Height = SCREEN_HEIGHT; // set the back buffer height
//    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // how swap chain is to be used
//    scd.OutputWindow = hWnd; // the window to be used
//    scd.SampleDesc.Count = 4; // how many multisamples
//    scd.Windowed = TRUE; // windowed/full-screen mode
//    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
//
//    //--> HERE
//    // create a device, device context and swap chain using the information in the
//    // scd struct
//    D3D11CreateDeviceAndSwapChain(NULL,
//        D3D_DRIVER_TYPE_HARDWARE,
//        NULL,
//        NULL,
//        NULL,
//        NULL,
//        D3D11_SDK_VERSION,
//        &scd,
//        &swapchain,
//        &dev,
//        NULL,
//        &devcon);
//
//    // get the address of the back buffer
//    CComPtr<ID3D11Texture2D> pBackBuffer;
//    swapchain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
//
//    // use the back buffer address to create the render target
//    dev->CreateRenderTargetView(pBackBuffer, NULL, &pBackbuffer);
//
//    // set the render target as the back buffer
//    devcon->OMSetRenderTargets(1, &pBackbuffer, NULL);
//
//    // Set the viewport
//    D3D11_VIEWPORT viewport;
//    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
//
//    viewport.TopLeftX = 0;
//    viewport.TopLeftY = 0;
//    viewport.Width = SCREEN_WIDTH;
//    viewport.Height = SCREEN_HEIGHT;
//
//    devcon->RSSetViewports(1, &viewport);
//
//    InitPipeline();
//    InitGraphics();
//
//    CComPtr<IDXGIDevice>    pDXGIDevice;
//    CComPtr<IDXGIAdapter>   pDXGIAdapter;
//    DXGI_ADAPTER_DESC       adapterDesc;
//
//    try
//    {
//        throw_if_fail(dev->QueryInterface(IID_PPV_ARGS(&pDXGIDevice)));
//        throw_if_fail(pDXGIDevice->GetAdapter(&pDXGIAdapter));
//        throw_if_fail(pDXGIAdapter->GetDesc(&adapterDesc));
//    }
//    catch (_com_error err)
//    {
//        OutputDebugString(err.ErrorMessage());
//    }
//    OutputDebugString(adapterDesc.Description);
//}
//
//// this is the function used to render a single frame
//void RenderFrame(void)
//{
//    // clear the back buffer to a deep blue
//    const float color[] = { 0.5f, 0.2f, 0.4f, 1.0f };
//    devcon->ClearRenderTargetView(pBackbuffer, color);
//
//    // select which vertex buffer to display
//    UINT stride = sizeof(VERTEX);
//    UINT offset = 0;
//    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
//
//    // select which primtive type we are using
//    devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//
//    // draw the vertex buffer to the back buffer
//    devcon->Draw(6, 0);
//
//    // switch the back buffer and the front buffer
//    swapchain->Present(0, 0);
//}

//// this is the function that creates the shape to render
//void InitGraphics()
//{
//    // create a triangle using the VERTEX struct
//    VERTEX OurVertices[] = {
//        { 0.0f, 0.5f, 0.0f, DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
//        { 0.45f, -0.5, 0.0f, DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
//        { -0.45f, -0.5f, 0.0f, DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
//
//        { 0.0f, 0.5f, 0.0f, DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
//        { 0.45f, -0.5, 0.0f, DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
//        { 0.90f, 0.5f, 0.0f, DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
//    };
//
//    // create the vertex buffer
//    D3D11_BUFFER_DESC bd;
//    ZeroMemory(&bd, sizeof(bd));
//
//    bd.Usage = D3D11_USAGE_DYNAMIC; // write access access by CPU and GPU
//    bd.ByteWidth = sizeof(VERTEX) * 6; // size is the VERTEX struct * 3
//    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as a vertex buffer
//    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow CPU to write in buffer
//
//    dev->CreateBuffer(&bd, NULL, &pVBuffer); // create the buffer
//
//    // copy the vertices into the buffer
//    D3D11_MAPPED_SUBRESOURCE ms;
//    devcon->Map(
//        pVBuffer,
//        NULL,
//        D3D11_MAP_WRITE_DISCARD,
//        NULL,
//        &ms); // map the buffer
//    memcpy(ms.pData, OurVertices, sizeof(OurVertices)); // copy the data
//    devcon->Unmap(pVBuffer, NULL); // unmap the buffer
//}
//
//// this function loads and prepares the shaders
//void InitPipeline()
//{
//    // load and compile the two shaders
//    ID3D10Blob *VS, *PS;
//    D3DCompileFromFile(
//        TEXT("shaders.shader"),
//        NULL,
//        NULL,
//        "VertShader",
//        "vs_5_0",
//        0,
//        0,
//        &VS,
//        NULL);
//    D3DCompileFromFile(
//        TEXT("shaders.shader"),
//        NULL,
//        NULL,
//        "PShader",
//        "ps_5_0",
//        0,
//        0,
//        &PS,
//        NULL);
//
//    // encapsulate both shaders into shader objects
//    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
//    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
//
//    // set the shader objects
//    devcon->VSSetShader(pVS, nullptr, 0);
//    devcon->PSSetShader(pPS, nullptr, 0);
//
//    // create the input layout object
//    D3D11_INPUT_ELEMENT_DESC ied[] = {
//        { "POSITION",
//            0,
//            DXGI_FORMAT_R32G32B32_FLOAT,
//            0,
//            0,
//            D3D11_INPUT_PER_VERTEX_DATA,
//            0 },
//        { "COLOR",
//            0,
//            DXGI_FORMAT_R32G32B32_FLOAT,
//            0,
//            sizeof(DirectX::XMFLOAT3),
//            D3D11_INPUT_PER_VERTEX_DATA,
//            0 },
//    };
//
//    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
//    devcon->IASetInputLayout(pLayout);
//}
