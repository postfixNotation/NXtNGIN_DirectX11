#include "Window/MainWindow.h"
#include "Utils/Utils.h"
#include "Gfx/Gfx.h"

#include <memory>

constexpr UINT kuiWidth = 1920, kuiHeight = 1080, kuiXpos = 0, kuiYpos = 0;

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    PWSTR pCmdLine,
    int nCmdShow)
{
    MainWindow window{ TEXT("NXtNGIN"), hInstance };

    if (!window.Create(TEXT("NXtNGIN - DirectX"), kuiWidth, kuiHeight, kuiXpos, kuiYpos))
    {
        return FALSE;
    }

    ShowWindow(window.Get(), nCmdShow);

    bool bGotMsg;
    MSG msg{};
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    std::unique_ptr<Gfx> pGfx = std::make_unique<Gfx>();

    pGfx->InitD3D(hInstance, window.Get(), kuiWidth, kuiHeight);
    pGfx->SetRenderTarget();
    pGfx->SetViewport(kuiWidth, kuiHeight);
    pGfx->InitShaders();
    pGfx->LoadVertices();
    pGfx->InitInputLayout();

    while (WM_QUIT != msg.message)
    {
        bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

        if (bGotMsg)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //renderer->Update();
            //renderer->Render();
            pGfx->Draw();

            //deviceResources->Present();
        }
    }

    return 0;
}
