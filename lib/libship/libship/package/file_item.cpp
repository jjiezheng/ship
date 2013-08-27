#include "stdafx.h"
#include "file_item.h"

#include "file_stream.h"

namespace ship
{
    FileItem::FileItem()
    {
    }

    FileItem::FileItem(const string& path)
    {
        data = make_shared<FileStream>(path);
        name = ExtractFileName(path);
    }

    FileItem::~FileItem()
    {
    }

    void FileItem::Install()
    {
    }

    void FileItem::Uninstall()
    {
    }
}