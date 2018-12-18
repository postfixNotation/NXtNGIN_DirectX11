#pragma once
#include <Windows.h>
#include <comdef.h>

namespace Utils
{
    inline void throw_if_fail(HRESULT hr);
}
