#pragma once

#include "xutil/stdafx.h"

#include <shlobj.h>

#include <yaml-cpp/yaml.h>
#include "xutil/all.h"
#include <boost/filesystem.hpp>

namespace ship
{
    using namespace std;
    using boost::unordered_map;
    
    using namespace boost::filesystem;
    typedef boost::filesystem::path Path;

    using namespace xutil;
}

#include "logs.h"