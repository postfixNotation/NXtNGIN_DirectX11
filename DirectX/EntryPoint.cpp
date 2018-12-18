#include "Window/MainWindow.h"
#include "Utils/Utils.h"

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    PWSTR pCmdLine,
    int nCmdShow)
{
    MainWindow window{ TEXT("NXtNGIN"), hInstance };

    if (!window.Create(TEXT("NXtNGIN - DirectX"), WS_OVERLAPPEDWINDOW))
    {
        return FALSE;
    }

    ShowWindow(window.Get(), nCmdShow);

    bool bGotMsg;
    MSG msg{};
    msg.message = WM_NULL;
    PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

    RECT rect;
    GetWindowRect(window.Get(), &rect);
    Utils::DebugLong(rect.right - rect.left);

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
            //deviceResources->Present();
        }
    }

    return 0;
}
