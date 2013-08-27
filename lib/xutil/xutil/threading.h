#pragma once

#include <boost/thread.hpp>

#include "threading/primitives.h"
#include "threading/atomic.h"
#include "threading/event.h"
#include "threading/task.h"
#include "threading/thread_pool.h"

namespace xutil
{
    using boost::thread_specific_ptr;
}
