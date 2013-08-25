#include "stdafx.h"
#include "stub.h"

#include "path.h"

#include "xutil/compress/archive_reader.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    string dir_name = CreateTempDirectory("install.");
    xutil::ArchiveReader reader(0, 0, 0);

    return 0;
}
