#include "stdafx.h"
#include "builder.h"

#include "file_bundle.h"

namespace ship
{
    Builder::Builder(const VarTablePtr& vars)
        : vars_(vars)
    {
    }

    PackagePtr Builder::Build(const YAML::Node& config)
    {
        auto package = make_shared<Package>();
        package->product.Add(config["product"]);
        
        auto data_node = config["data"];
        ImportData(data_node);
        return package;
    }

    void Builder::ImportData(const YAML::Node& node, const string& install_location /* = "" */)
    {
        for (auto i = node.begin(); i != node.end(); ++i)
        {
            string key = i->first.as<string>();
            if (key == "bundle")
            {
                string custom_location = install_location;
                if (custom_location.empty())
                {
                    custom_location = "@app";
                }
                FileBundle bundle(custom_location, vars_);
                bundle.Import(i->second);
                for (auto i = bundle.files.begin(); i != bundle.files.end(); ++i)
                {
                    FileItemPtr file = *i;
                    cout << "    " << file->name << endl;
                }
            }
            else
            {
                string custom_location = key;
                if (!install_location.empty())
                {
                    custom_location = CombinePath(install_location, custom_location);
                }
            }
        }
    }
}