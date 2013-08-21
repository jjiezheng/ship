#pragma once

#include "package_item.h"
#include "product_info.h"

namespace ship
{
    class Package
    {
    public:
        ProductInfo product;

    private:
        typedef vector<PackageItemPtr> PackageContent;
        PackageContent content_;
    };

    typedef shared_ptr<Package> PackagePtr;
}