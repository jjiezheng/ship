#pragma once

#include "targetver.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <exception>
#include <functional>
#include <iterator>
#include <algorithm>


#include <boost/noncopyable.hpp>
#include <boost/format.hpp>
#include <boost/scoped_array.hpp>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>

#include <windows.h>
#include <tchar.h>

namespace xutil
{
    using namespace std;

    using boost::noncopyable;
    using boost::format;
    using boost::str;
    using boost::scoped_array;
    using boost::unordered_set;
    using boost::unordered_map;

    using boost::starts_with;
}

#include "pointers.h"
#include "exception.h"
#include "strutil.h"
#include "generic_exceptions.h"
#include "logs.h"