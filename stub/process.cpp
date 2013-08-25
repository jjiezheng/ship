#include "stdafx.h"
#include "process.h"

#include "package_info.h"

string GetApplicationPath()
{
    char path[MAX_PATH + 1];
    GetModuleFileName(NULL, path, MAX_PATH + 1);
    return string(path);
}

void Unpack(const string& dir)
{
    int offset = GetPackageDataOffset();
    int size = GetPackageDataSize();
    int buf_size = min(size, 0x200000);
    char* buf = new char[buf_size];

    string app_path = GetApplicationPath();
    HANDLE file_handle = CreateFile(app_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    SetFilePointer(file_handle, offset, 0, FILE_BEGIN);

    DWORD bytes_read = buf_size;
    do
    {
        ReadFile(file_handle, buf, buf_size, &bytes_read, NULL);
        if (bytes_read > 0)
        {

        }
    }
    while (bytes_read == buf_size);

    CloseHandle(file_handle);
    delete[] buf;
}