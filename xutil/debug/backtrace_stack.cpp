#include "stdafx.h"
#include "backtrace_stack.h"

namespace xutil
{
    ostream& operator << (ostream& os, const BacktraceStack& stack)
    {
        for (auto i = stack.begin(); i != stack.end(); ++i)
        {
            const string& stack_entry = *i;
            os << stack_entry << endl;
        }

        return os;
    }
}