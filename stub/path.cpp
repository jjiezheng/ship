#include "stdafx.h"
#include "path.h"

string GetTempFolder()
{
    const int buf_size = MAX_PATH + 1;
    TCHAR buf[buf_size];
    int len = GetTempPath(buf_size, buf);
    return string(buf, len);
}

string CreateTempDirectory(const char* prefix)
{
    string tmp = GetTempFolder();
    char last = tmp[tmp.size() -1];
    if (last != '\\' && last != '/')
    {
        tmp.append("\\");
    }
    tmp.append(prefix);

    const int buf_size = 32;
    char buf[buf_size];
    unsigned time = ::GetTickCount();
    for (;;)
    {
        ++time;
        _snprintf(buf, buf_size, "%x", time);
        string dir_path = tmp + buf;

        if (CreateDirectory(dir_path.c_str(), NULL))
        {
            return dir_path;
        }
        else if (GetLastError() != ERROR_ALREADY_EXISTS)
        {
            return "";
        }
    }
}