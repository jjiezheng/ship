#include "stdafx.h"
#include "trace.h"

namespace xutil
{
    bool xtrace(TCHAR* format, ...)
    {
       TCHAR buffer[1000];

       va_list argptr;
       va_start(argptr, format);
       wvsprintf(buffer, format, argptr);
       va_end(argptr);

       OutputDebugString(buffer);
       return true;
    }
}