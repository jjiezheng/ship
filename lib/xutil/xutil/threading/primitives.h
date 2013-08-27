#pragma once

#include <boost/thread.hpp>

namespace xutil
{
    typedef boost::thread Thread;
    typedef shared_ptr<Thread> ThreadPtr;

    typedef boost::thread_group ThreadGroup;
    
    typedef boost::mutex Mutex; // non recursive
    typedef boost::unique_lock<Mutex> UniqueLock;
}