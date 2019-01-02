#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <comdef.h>
#include <cstdio>

namespace Utils
{
    inline void throw_if_fail(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw _com_error(hr);
        }
    }

    inline void DebugLong(INT iMessage)
    {
        static TCHAR output[32];
        swprintf_s(output, TEXT("DebugLong: %i\n"), iMessage);
        OutputDebugString(output);
    }
}
