#pragma once

#include <boost/interprocess/detail/atomic.hpp>

namespace xutil
{
    inline unsigned atomic_increment(volatile unsigned* value)
    {
        return boost::interprocess::ipcdetail::atomic_inc32(value);
    }

    inline unsigned atomic_decrement(volatile unsigned* value)
    {
        return boost::interprocess::ipcdetail::atomic_dec32(value);
    }
}