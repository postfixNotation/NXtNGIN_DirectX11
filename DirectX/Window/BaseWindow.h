#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    BaseWindow(HINSTANCE hInstance) :
        m_hInstance(hInstance)
    {
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE *pThis{ nullptr };

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<DERIVED_TYPE*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0,
        HMENU hMenu = 0
    )
    {
        WNDCLASSEX wc{};
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.hIcon = NULL;
        wc.hCursor = LoadCursor(NULL, IDC_CROSS);

        RegisterClassEx(&wc);

        RECT rect{ 0, 0, nWidth, nHeight };
        AdjustWindowRect(
            &rect,
            dwStyle,
            FALSE);

        m_hwnd = CreateWindowEx(
            dwExStyle,
            ClassName(),
            lpWindowName,
            dwStyle,
            x,
            y,
            nWidth,
            nHeight,
            hWndParent,
            hMenu,
            GetModuleHandle(NULL),
            this
        );

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND Get() const { return m_hwnd; }

protected:
    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd{ NULL };
private:
    HINSTANCE m_hInstance;
};
