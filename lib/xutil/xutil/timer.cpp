#include "stdafx.h"
#include "timer.h"

#ifndef WIN32
    #include <time.h>
#endif

namespace xutil
{
    namespace 
    {
        inline uint64_t GetFrequency()
        {
#ifdef WIN32
            LARGE_INTEGER large_freq;
            ::QueryPerformanceFrequency(&large_freq);
            return large_freq.QuadPart / 1000; // in milliseconds
#else
            return 1000000;
#endif
        }

        inline uint64_t GetCurrentTime()
        {
#ifdef WIN32
            LARGE_INTEGER large_time;
            ::QueryPerformanceCounter(&large_time);

            // time in ticks
            return large_time.QuadPart;
#else
            timespec time;
            clock_gettime(CLOCK_MONOTONIC, &time);

            // time in nanoseconds
            return (uint64_t)(time.tv_sec) * (uint64_t)(1000000000) + (uint64_t)(time.tv_nsec);
#endif
        }
    }

    Timer::Timer()
    {
        freq_ = GetFrequency();
        Reset();
    }

    double Timer::ElapsedSeconds() const
    {
        return ElapsedMilliseconds() / 1000.0;
    }

    uint64_t Timer::ElapsedMilliseconds() const
    {
        return GetMillisecondsTimeMark() - start_timemark_;
    }

    void Timer::Reset()
    {
        start_timemark_ = GetMillisecondsTimeMark();
    }

    double Timer::ResetSeconds()
    {
        return ResetMilliseconds() / 1000.0;
    }

    uint64_t Timer::ResetMilliseconds()
    {
        uint64_t timemark =  GetMillisecondsTimeMark();
        uint64_t interval = timemark - start_timemark_;
        start_timemark_ = timemark;
        
        return interval;
    }

    uint64_t Timer::GetMillisecondsTimeMark() const
    {
        return GetCurrentTime() / freq_;
    }
}
