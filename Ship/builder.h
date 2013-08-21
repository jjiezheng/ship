#pragma once

#include "package/package.h"

namespace ship
{
    class Builder
    {
    public:
        PackagePtr Build(const YAML::Node& config, const VarTable& vars);
    };
}