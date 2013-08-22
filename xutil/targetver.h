#pragma once

#include <WinSDKVer.h>

#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0502   // Windows XP
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <winsock2.h>
#include <windows.h>
