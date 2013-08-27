#pragma once

namespace xutil
{
    class BacktraceStack : public vector<string>
    {
    };

    ostream& operator << (ostream& os, const BacktraceStack& stack);
}