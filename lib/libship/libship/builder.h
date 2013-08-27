#pragma once

#include "package/package.h"
#include "var_table.h"

namespace ship
{
    class Builder
    {
    public:
        Builder(const VarTablePtr& vars);
        PackagePtr Build(const YAML::Node& config);
    private:
        void ImportData(Package& package, const YAML::Node& node, const string& install_location = "");
        VarTablePtr vars_;
    };
}