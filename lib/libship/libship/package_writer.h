#pragma once

#include "package/package.h"

namespace ship
{
    class PackageWriter
    {
    public:
        PackageWriter(const string& stub_path);

        void Compose(const string& path, const PackagePtr& package);
        const string& stub_path() const { return stub_path_; }

        vector<string> installer_content;
    private:
        string stub_path_;
    };
}