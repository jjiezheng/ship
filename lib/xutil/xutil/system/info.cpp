#include "stdafx.h"
#include "info.h"

namespace xutil
{
    int GetNumberOfProcessors()
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        return info.dwNumberOfProcessors;
    }
}