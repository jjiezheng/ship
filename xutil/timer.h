#pragma once

namespace xutil
{
    class Timer
    {
    public:
        Timer();

        double ElapsedSeconds() const;
        uint64_t ElapsedMilliseconds() const;

        void Reset();
        double ResetSeconds();
        uint64_t ResetMilliseconds();

    private:
        uint64_t GetMillisecondsTimeMark() const;

        uint64_t start_timemark_;
        uint64_t freq_;
    };
}