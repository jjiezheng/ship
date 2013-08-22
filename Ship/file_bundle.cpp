#include "stdafx.h"
#include "file_bundle.h"

namespace ship
{
    FileBundle::FileBundle(const string& install_location, const VarTablePtr& vars)
    {
        install_location_ = install_location;
        vars_ = vars;
    }

    void FileBundle::Import(const YAML::Node& config)
    {
        switch (config.Type())
        {
        case YAML::NodeType::Scalar:
            ImportScalarNode(config);
            break;
        case YAML::NodeType::Sequence:
            for (auto i = config.begin(); i != config.end(); ++i)
            {
                const YAML::Node& node = *i;
                switch(node.Type())
                {
                    case YAML::NodeType::Scalar:
                        ImportScalarNode(node);
                        break;
                    case YAML::NodeType::Map:
                        for (auto k = node.begin(); k != node.end(); ++k)
                        {
                            auto file = ImportScalarNode(k->second);
                            file->name = k->first.as<string>();
                        }
                        break;
                }
            }
            break;
        case YAML::NodeType::Map:
            ImportCompositeBundle(config);
            break;
        }
    }

    FileItemPtr FileBundle::ImportScalarNode(const YAML::Node& config)
    {
        string path = config.as<string>();
        auto file = make_shared<FileItem>(path);
        file->install_location = install_location_;
        files.push_back(file);
        return file;
    }

    void FileBundle::ImportCompositeBundle(const YAML::Node& config)
    {
        string dir = vars_->Expand(config["dir"]);
        vector<string> dir_files = GetFileList(dir);

        vector<string> include_masks;
        auto include_node = config["include"];
        if (include_node.IsDefined())
        {
            for (auto m = include_node.begin(); m != include_node.end(); ++m)
            {
                include_masks.push_back(m->as<string>());
            }
        }

        vector<string> exclude_masks;
        auto exclude_node = config["exclude"];
        if (exclude_node.IsDefined())
        {
            for (auto m = exclude_node.begin(); m != exclude_node.end(); ++m)
            {
                exclude_masks.push_back(m->as<string>());
            }
        }

        for (auto i = dir_files.begin(); i != dir_files.end(); ++i)
        {
            const string& file_name = *i;
            if (include_masks.empty() || PathMatchesWildcard(file_name, include_masks))
            {
                if (!PathMatchesWildcard(file_name, exclude_masks))
                {
                    string file_path = CombinePath(dir, file_name);
                    FileItemPtr file = make_shared<FileItem>(file_path);
                    file->install_location = install_location_;
                    files.push_back(file);
                }
            }
        }
    }
}