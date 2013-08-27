#include "stdafx.h"
#include "environment.h"

namespace ship
{
    namespace 
    {
        string GetSpecialFolder(int csidl)
        {
            char buf[MAX_PATH + 1];
            if (!SHGetSpecialFolderPath(0, buf, csidl, FALSE))
            {
                return string();
            }
            else
            {
                return string(buf);
            }
        }

        string GetTempFolder()
        {
            const int buf_size = MAX_PATH + 1;
            char buf[buf_size];
            int len = GetTempPath(buf_size, buf);
            return string(buf, len - 1);
        }
    }


    Environment::Environment()
    {
        Reset();
    }

    Environment::~Environment()
    {
    }

    void Environment::Reset()
    {
        Clear();

        set("programs", CSIDL_PROGRAMS);
        set("desktop",  CSIDL_DESKTOPDIRECTORY);
        set("system",   CSIDL_SYSTEM);
        set("app_data", CSIDL_APPDATA);

        set("temp", GetTempFolder());
    }

    string Environment::Expand(const string& value) const
    {
        return ExpandKey('@', value);        
    }

    void Environment::set(const string& key, int csidl)
    {
        set(key, GetSpecialFolder(csidl));
    }
}