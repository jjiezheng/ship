#pragma once

#include "backtrace_stack.h"

namespace xutil
{
    // SymbolContext is a threadsafe singleton that wraps the DbgHelp Sym* family of functions.
    // The Sym* family of functions may only be invoked by one thread at a time.
    // SymbolContext code may access a symbol server over the network while holding the lock for this singleton.
    // In the case of high latency, this code will adversly affect performance.
    class SymbolServer : noncopyable
    {
    public:
        static SymbolServer* instance();

        bool filter_stubs;
        DWORD init_error() const;

        bool DumpContext(BacktraceStack* stack, PCONTEXT context, HANDLE thread = 0, int skip = 0);
        BacktraceStack DumpContext(PCONTEXT context, HANDLE thread = 0, int skip = 0);

        bool DumpTrace(BacktraceStack* stack, const void* const* trace,  int count);
        BacktraceStack DumpTrace(const void* const* trace,  int count);


    private:
        SymbolServer();
        DWORD init_error_;
        vector<string> stack_startup_entries_;
        vector<string> stack_throw_entries_;
    };
}