#pragma once

#include "config_data.h"

namespace ship
{
    class VarTable : public ConfigData
    {
    public:
        virtual ~VarTable();

        string Expand(const string& value) const;
    };
}