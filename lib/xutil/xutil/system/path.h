#pragma once

namespace xutil
{
    string ExtractFileName(const string& path);
    string ExtractParentDirectory(const string& path);
    string CombinePath(const string& path, const string& s);

    bool PathMatchesWildcard(const string& path, const string& wildcard);
    bool PathMatchesWildcard(const string& path, const vector<string>& wildcards);
    vector<string> GetFileList(const string& dir);

    string GetApplicationPath();
    string GetApplicationDirectory();
}