#include "Window/MainWindow.h"
#include "Utils/Utils.h"
#include "Gfx/Gfx.h"

constexpr UINT kuiWidth = 1920, kuiHeight = 1080, kuiXpos = 0, kuiYpos = 0;

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    PWSTR pCmdLine,
    int nCmdShow)
{
    MainWindow window{ TEXT("NXtNGIN"), hInstance };

    if (!window.Create(TEXT("NXtNGIN - DirectX"), kuiWidth / 2, kuiHeight / 2, kuiXpos, kuiYpos))
    {
        return FALSE;
    }

    ShowWindow(window.Get(), nCmdShow);

    bool bGotMsg;
    MSG msg{};
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    Gfx gfx;

    gfx.InitD3D(hInstance, window.Get(), kuiWidth, kuiHeight);
    gfx.SetRenderTarget();
    gfx.SetViewport(kuiWidth, kuiHeight);
    gfx.CompileShaders();

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

            gfx.RenderFrame();

            //deviceResources->Present();
        }
    }

    return 0;
}
