#pragma once

#include "config_data.h"

namespace ship
{
    class Environment : public ConfigData
    {
    public:
        Environment();
        virtual ~Environment();
        
        void Reset();
        string Expand(const string& value) const;
        using ConfigData::set;
    protected:
        void set(const string& key, int csidl);
    };
}