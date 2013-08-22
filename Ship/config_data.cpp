#include "stdafx.h"
#include "config_data.h"

namespace ship
{
    namespace 
    {
        inline bool IsAlphaNumeric(char c)
        {
            return (c >= 'A' && c <= 'Z') || 
                   (c >= 'a' && c <= 'z') || 
                   (c >= '0' && c <= '9') ||
                   c == '_' || 
                   c == '~';
        }

        string GetVarName(const string& s, int start)
        {
            int i = start;
            for (; i < s.length(); ++i)
            {
                if (!IsAlphaNumeric(s[i]))
                {
                    break;
                }
            }

            int len = i - 1;
            if (len > 0)
            {
                return string(s.c_str() + start, len);
            }
            else
            {
                return string();
            }
        }
    }

    ConfigData::~ConfigData()
    {
    }

    ConfigData::Iterator ConfigData::begin()
    {
        return storage_.begin();
    }

    ConfigData::Iterator ConfigData::end()
    {
        return storage_.end();
    }

    ConfigData::ConstIterator ConfigData::begin() const
    {
        return storage_.begin();
    }

    ConfigData::ConstIterator ConfigData::end() const
    {
        return storage_.end();
    }

    string& ConfigData::operator[](const string& key)
    {
        return storage_[key];
    }

    const string& ConfigData::get(const string& key) const
    {
        auto i = storage_.find(key);
        return i->second;
    }

    const string& ConfigData::get(const string& key, const string& default_value) const
    {
        auto i = storage_.find(key);
        if (i == storage_.end())
        {
            return default_value;
        }
        else
        {
            return i->second;
        }
    }

    void ConfigData::set(const string& key, const string& value)
    {
        storage_[key] = value;
    }

    void ConfigData::Add(const ConfigData& other)
    {
        for (auto i = other.begin(); i != other.end(); ++i)
        {
            set(i->first, i->second);
        }
    }

    void ConfigData::Add(const YAML::Node& node)
    {
        for (auto i = node.begin(); i != node.end(); ++i)
        {
            set(i->first.as<string>(), i->second.as<string>());
        }
    }

    void ConfigData::Clear()
    {
        storage_.clear();
    }

    string ConfigData::ExpandKey(char prefix, const string& value) const
    {
        string res;
        int name_start = 0;
        int name_end = 0;

        while (name_start >= 0)
        {
            name_start = value.find(prefix, name_end);
            if (name_start >= 0)
            {
                res.append(value.c_str() + name_end, value.c_str() + name_start);
                ++name_start;
                string name = GetVarName(value, name_start);
                string value = get(name);
                res.append(value);

                name_end = name_start + name.length();
            }
        }

        res.append(value.begin() + name_end, value.end());
        return res;
    }
}