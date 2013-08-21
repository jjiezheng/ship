#pragma once

namespace ship
{
    class ConfigData
    {
    public:
        typedef unordered_map<string, string> Storage;
        typedef Storage::iterator Iterator;
        typedef Storage::const_iterator ConstIterator;

        virtual ~ConfigData();

        Iterator begin();
        Iterator end();
        ConstIterator begin() const;
        ConstIterator end() const;

        void Clear();
        void Add(const ConfigData& other);
        void Add(const YAML::Node& node);
        string& operator[](const string& key);

        const string& get(const string& key) const;
        const string& get(const string& key, const string& default_value) const;

        void set(const string& key, const string& value);
    
    protected:
        string ExpandKey(char prefix, const string& value) const;
        Storage storage_;
    };

    typedef shared_ptr<ConfigData> ConfigDataPtr;
}
