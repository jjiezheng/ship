#pragma once

#include "../config_data.h"

namespace ship
{
    class ProductInfo : public ConfigData
    {
    public:
        virtual ~ProductInfo();

        string name() const { return get("name", ""); }
        string manufacturer() const { return get("manufacturer", ""); }
        string version() const { return get("version", ""); }
    };
}