#pragma once

#include "package_item.h"
#include "data_stream.h"

namespace ship
{
    class FileItem : public PackageItem
    {
    public:
        FileItem();
        FileItem(const string& path);
        virtual ~FileItem();

        virtual void Install();
        virtual void Uninstall();

        string name;
        string install_location;

        DataStreamPtr data;
    };

    typedef shared_ptr<FileItem> FileItemPtr;
}