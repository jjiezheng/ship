#include "stdafx.h"
#include "config.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

using namespace boost::property_tree;

namespace xutil
{
    static ConfigFileFormat detect_file_format(const string& filename)
    {
        if (string_ends_with(filename, ".ini"))
        {
            return CONFIG_FORMAT_INI;
        }
        else if (string_ends_with(filename, ".xml"))
        {
            return CONFIG_FORMAT_XML;
        }
        else if (string_ends_with(filename, ".json"))
        {
            return CONFIG_FORMAT_JSON;
        }
        else
        {
            return CONFIG_FORMAT_INFO;
        }
    }


    void Config::SetProperties(const ConfigProperties& props)
    {
        properties_ = props;
    }


    bool Config::Load(const string& path, ConfigFileFormat format /* = CONFIG_FORMAT_UNDEFINED */)
    {
        properties_.clear();

        if (format == CONFIG_FORMAT_UNDEFINED)
        {
            format = detect_file_format(path);
        }

        switch (format)
        {
        case CONFIG_FORMAT_INI:
            ini_parser::read_ini(path, properties_);
            return true;
        case CONFIG_FORMAT_XML:
            xml_parser::read_xml(path, properties_);
            return true;
        case CONFIG_FORMAT_JSON:
            json_parser::read_json(path, properties_);
            return true;
        case CONFIG_FORMAT_INFO:
            info_parser::read_info(path, properties_);
            return true;
        default:
            return false;
        };
    }

    bool Config::Save(const string& path, ConfigFileFormat format /* = CONFIG_FORMAT_UNDEFINED */) const
    {
        if (format == CONFIG_FORMAT_UNDEFINED)
        {
            format = detect_file_format(path);
        }

        switch (format)
        {
        case CONFIG_FORMAT_INI:
            ini_parser::write_ini(path, properties_);
            return true;
        case CONFIG_FORMAT_XML:
            xml_parser::write_xml(path, properties_);
            return true;
        case CONFIG_FORMAT_JSON:
            json_parser::write_json(path, properties_);
            return true;
        case CONFIG_FORMAT_INFO:
            info_parser::write_info(path, properties_);
            return true;
        default:
            return false;
        };
    }

    const ConfigProperties* Config::get_child(const string& key) const
    {
        const auto& optional_props = properties_.get_child_optional(key);
        if (optional_props)
        {
            return &optional_props.get();
        }
        else
        {
            return nullptr;
        }
    }
}