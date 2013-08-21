#include "stdafx.h"
#include "builder.h"

#include "package/file_item.h"

namespace ship
{
    class FileBundle
    {
    public:
        FileBundle(const string& install_location, const YAML::Node& config, const VarTable& vars)
        {
            switch(config.Tag())
            {
            case YAML::NodeType::Scalar:
                {
                    string path = config.as<string>();
                    auto file = make_shared<FileItem>(path);
                    file->install_location = install_location;
                    files.push_back(file);
                }
                break;
            case YAML::NodeType::Sequence:
                {
                    for (auto i = config.begin(); i != config.end(); ++i)
                    {
                    }
                }
                break;
            case YAML::NodeType::Map:
                {
                }
                break;
            }
        }

        vector<FileItemPtr> files;
    };

    PackagePtr Builder::Build(const YAML::Node& config, const VarTable& vars)
    {
        auto package = make_shared<Package>();
        package->product.Add(config["product"]);
        
        auto filesNode = config["files"];
        for (auto i = filesNode.begin(); i != filesNode.end(); ++i)
        {
            string key = i->first.as<string>();
            if (key == "bundle")
            {

            }
            const YAML::Node& value = i->second;

        }
        return package;
    }
}