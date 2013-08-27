#include "stdafx.h"
#include "path.h"

#include "shlwapi.h"

namespace xutil
{
#ifdef WIN32
    string GetApplicationPath()
    {
        char path[MAX_PATH + 1];
        if (!GetModuleFileName(NULL, path, MAX_PATH + 1))
        {
            throw SystemError("Failed to retrieve applicatin path");
        }

        return string(path);
    }
#endif

    string ExtractFileName(const string& path)
    {
        unsigned pos = path.find_last_of("/\\");
        if (pos == string::npos)
        {
            return path;
        }

        return path.substr(pos + 1);
    }

    string ExtractParentDirectory(const string& path)
    {
        unsigned count = path.length();
        if (!count)
        {
            return "";
        }

        char last_char = path[count - 1];
        if (last_char == '/' || last_char == '\\')
        {
            count--;
        }

        unsigned pos = path.find_last_of("/\\", 0, count);
        if (pos == string::npos)
        {
            return "";
        }

        return path.substr(0, pos);
    }

    string CombinePath(const string& path, const string& s)
    {
        return path + "\\" + s;
    }

    vector<string> GetFileList(const string& dir)
    {
        vector<string> res;
        string mask = dir + "\\*";

        WIN32_FIND_DATA ffd;
        HANDLE hFind;
        hFind = FindFirstFile(mask.c_str(), &ffd);
        if (hFind != INVALID_HANDLE_VALUE) 
        {
            do
            {
                if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    res.push_back(ffd.cFileName);
                }
            }
            while (FindNextFile(hFind, &ffd) != 0);
        }
        return res;
    }

    bool PathMatchesWildcard(const string& path, const string& wildcard)
    {
        return !!PathMatchSpec(path.c_str(), wildcard.c_str());
    }

    bool PathMatchesWildcard(const string& path, const vector<string>& wildcards)
    {
        for (auto i = wildcards.begin(); i != wildcards.end(); ++i)
        {
            const string& wildcard = *i;
            if (PathMatchesWildcard(path, wildcard))
            {
                return true;
            }
        }
        return false;
    }

    string GetApplicationDirectory()
    {
        string app_path = GetApplicationPath();
        string app_dir = ExtractParentDirectory(app_path);
        return app_dir;
    }
}
