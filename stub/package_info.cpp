#include "stdafx.h"
#include "package_info.h"

static char* PKG_DATA_OFFSET = "!PKG_DATA_OFFSET!0000000000";
static char* PKG_DATA_SIZE = "!PKG_DATA_SIZE!0000000000";
static char* PKG_DATA_EXE_NAME = "!PKG_DATA_EXE_NAME!setup.exe\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

static string ExtractPackageValue(const string& encoded_value)
{
    int mark_pos = encoded_value.find_last_of('!');
    return encoded_value.substr(mark_pos + 1);
}

static int ExtractPackageIntValue(const string& encoded_value)
{
    int mark_pos = encoded_value.find_last_of('!');
    const char* str_value = encoded_value.c_str() + mark_pos + 1;
    return atoi(str_value);
}

int GetPackageDataOffset()
{
    return ExtractPackageIntValue(PKG_DATA_OFFSET);
}

int GetPackageDataSize()
{
    return ExtractPackageIntValue(PKG_DATA_SIZE);
}

string GetPackageExeName()
{
    return ExtractPackageValue(PKG_DATA_EXE_NAME);
}