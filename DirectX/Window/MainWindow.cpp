#include "MainWindow.h"

MainWindow::MainWindow(PCWSTR lpszName, HINSTANCE hInstance) :
    BaseWindow(hInstance),
    m_lpszName(lpszName)
{
}

PCWSTR MainWindow::ClassName() const { return m_lpszName; }

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    case WM_CLOSE:
    {
        HMENU hMenu;
        hMenu = GetMenu(m_hWnd);
        if (hMenu != NULL)
        {
            DestroyMenu(hMenu);
        }
        DestroyWindow(m_hWnd);
        UnregisterClass(
            ClassName(),
            m_hInstance
        );
        return 0;
    }

    //case WM_PAINT:
    //{
    //    PAINTSTRUCT ps;
    //    HDC hdc = BeginPaint(m_hWnd, &ps);
    //    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    //    EndPaint(m_hWnd, &ps);
    //    return 0;
    //}

    default:
        return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
    return TRUE;
}
