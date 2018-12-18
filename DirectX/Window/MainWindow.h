#pragma once
#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow>
{
public:
    MainWindow(PCWSTR lpszName, HINSTANCE hInstance);
    PCWSTR ClassName() const;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    PCWSTR m_lpszName;
};
