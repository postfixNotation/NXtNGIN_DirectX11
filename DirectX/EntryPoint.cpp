#include "Window/MainWindow.h"

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    PWSTR pCmdLine,
    int nCmdShow)
{
    MainWindow window{ TEXT("NXtNGIN"), hInstance };

    if (!window.Create(TEXT("NXtNGIN - DirectX"), WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(window.Get(), nCmdShow);

    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
