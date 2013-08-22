#pragma once

namespace xutil
{
    class Backtrace
    {
    public:
        static const int DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT = 1;
        static const int DEFAULT_STACK_DEPTH = 50;

        typedef vector<string> Stack;

        Backtrace(int exclude_cnt = DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT, int depth = DEFAULT_STACK_DEPTH);
        Stack GetStack() const;

    private:
        vector<void*> backtrace_;
        size_t trace_size_;
    };

    inline ostream& operator<<(ostream& os, const Backtrace& bt)
    {
        Backtrace::Stack stack = bt.GetStack();
        copy(begin(stack), end(stack), ostream_iterator<string>(os, "\n"));
        return os;
    }
}