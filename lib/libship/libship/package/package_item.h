#pragma once

namespace ship
{
    class PackageItem
    {
    public:
        virtual ~PackageItem();

        virtual void Install() = 0;
        virtual void Uninstall() = 0;
    };

    typedef shared_ptr<PackageItem> PackageItemPtr;
}