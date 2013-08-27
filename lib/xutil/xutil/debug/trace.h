#pragma once

namespace xutil
{
#ifdef _DEBUG
    #define DBG_TRACE dbg_trace
#else
    #define DBG_TRACE 0
#endif

    bool xtrace(TCHAR* format, ...);
}