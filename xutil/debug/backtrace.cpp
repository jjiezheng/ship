#include "stdafx.h"
#include "backtrace.h"

#ifdef WIN32
#include <dbghelp.h>
#else
#include <execinfo.h>
#include <cxxabi.h>
#endif

#include "symbol_server.h"
#include "../system/path.h"
#include "../threading/primitives.h"

namespace xutil
{
#ifdef WIN32
    Backtrace::Backtrace(int exclude_cnt /* = DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT */, int depth /* = DEFAULT_STACK_DEPTH */)
        : backtrace_(depth)
    {
        trace_size_ = CaptureStackBackTrace(exclude_cnt, depth, &backtrace_[0], NULL);
    }

    BacktraceStack Backtrace::GetStack() const
    {
        BacktraceStack result;
        try
        {
            SymbolServer::instance()->DumpTrace(&result, &backtrace_[0], trace_size_);
        }
        catch (boost::exception& e)
        {
            cerr << endl << "--- Error dumping stack trace --- " << endl;
            cerr << diagnostic_information(e) << endl << endl;
        }
        catch (exception& e)
        {
            cerr << endl << "--- Error dumping stack trace --- " << endl;
            cerr << e.what() << endl << endl;
        }
        catch (...)
        {
            cerr << endl << "--- Error dumping stack trace --- " << endl;
            cerr << "UNKNOWN EXCEPTION" << endl << endl;
        }

        return result;
    }

#else
    Backtrace::Backtrace(int exclude_cnt /* = DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT */, int depth /* = DEFAULT_STACK_DEPTH */)
    {
        void* bt[depth];
        size_t size = ::backtrace(bt, depth);
        if (size > size_t(exclude_cnt))
        {
            trace_size_ = size - exclude_cnt;
            backtrace_.resize(trace_size_);
            copy(bt + exclude_cnt, bt + size, &backtrace_[0]);
        }
    }

    BacktraceStack Backtrace::GetStack() const
    {
        BacktraceStack result;
        if (trace_size_ > 0)
        {
            if (char** strings = backtrace_symbols(&backtrace_[0], trace_size_))
            {
                for (size_t i = 0; i < trace_size_; ++i)
                {
                    // Try to demangle current name
                    if (char* open_bracket = strrchr(strings[i], '('))
                    {
                        if (char* plus_sign = strchr(open_bracket, '+'))
                        {
                            *plus_sign = 0;
                            int status;
                            char* n = abi::__cxa_demangle(++open_bracket, 0, 0, &status);
                            *plus_sign = '+';
                            if (!status)
                            {
                                string r(strings[i], open_bracket - strings[i]);
                                r += n + string(plus_sign);
                                result.push_back(r);
                                free(n);
                                continue;
                            }
                        }
                    }
                    result.push_back(strings[i]);
                }
                free(strings);
            }
        }
        return result;
    }
#endif

}