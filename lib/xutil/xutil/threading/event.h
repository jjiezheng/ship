#pragma once

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace xutil
{
    class Event
    {
    public:
        void wait()
        {
            boost::unique_lock<boost::mutex> lock(mutex_);
            cond_.wait(lock);
        }

        bool wait(int timeout_in_milliseconds)
        {
            boost::unique_lock<boost::mutex> lock(mutex_);
            return cond_.timed_wait(lock, boost::posix_time::milliseconds(timeout_in_milliseconds));
        }

        void notify_one()
        {
            cond_.notify_one();
        }

        void notify_all()
        {
            cond_.notify_all();
        }

        boost::mutex& mutex() { return mutex_;}
        boost::condition_variable& cond() { return cond_; }

    private:
        boost::mutex mutex_;
        boost::condition_variable cond_;
    };

    typedef shared_ptr<Event> EventPtr;
}