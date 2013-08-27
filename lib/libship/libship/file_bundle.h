#pragma once

#include "package/file_item.h"
#include "var_table.h"

namespace ship
{
    class FileBundle
    {
    public:
        FileBundle(const string& install_location, const VarTablePtr& vars);

        vector<FileItemPtr> files;
        void Import(const YAML::Node& config);

    private:
        string install_location_;
        VarTablePtr vars_;

        FileItemPtr ImportScalarNode(const YAML::Node& config);
        void ImportCompositeBundle(const YAML::Node& config);
    };
}