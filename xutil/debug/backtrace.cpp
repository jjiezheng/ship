#include "stdafx.h"
#include "backtrace.h"

#ifdef WIN32
#include <dbghelp.h>
#else
#include <execinfo.h>
#include <cxxabi.h>
#endif

#include "../system/path.h"
#include "../threading/primitives.h"

namespace xutil
{
#ifdef WIN32
    // SymbolContext is a threadsafe singleton that wraps the DbgHelp Sym* family
    // of functions.  The Sym* family of functions may only be invoked by one
    // thread at a time.  SymbolContext code may access a symbol server over the
    // network while holding the lock for this singleton.  In the case of high
    // latency, this code will adversly affect performance.
    class SymbolContext : noncopyable
    {
    public:
        static SymbolContext* GetInstance() 
        {
            if (!g_instance_)
            {
                UniqueLock lock(lock_);
                if (!g_instance_)
                {
                    g_instance_ = new SymbolContext();
                }
            }
            return g_instance_;
        }

        // Returns the error code of a failed initialization.
        DWORD init_error() const { return init_error_; }

        // For the given trace, attempts to resolve the symbols, and output a trace
        // to the ostream os.  The format for each line of the backtrace is:
        //
        //    <tab>SymbolName[0xAddress+Offset] (FileName:LineNo)
        //
        // This function should only be called if Init() has been called.  We do not
        // LOG(FATAL) here because this code is called might be triggered by a
        // LOG(FATAL) itself.
        void DumpTrace(const void* const* trace,  int count, Backtrace::Stack* stack)
        {
            UniqueLock lock(lock_);

            for (int i = 0; i < count; ++i) 
            {
                const int kMaxNameLength = 256;
                DWORD_PTR frame = reinterpret_cast<DWORD_PTR>(trace[i]);

                // Code adapted from MSDN example: http://msdn.microsoft.com/en-us/library/ms680578(VS.85).aspx
                ULONG64 buffer[(sizeof(SYMBOL_INFO) +
                                kMaxNameLength * sizeof(wchar_t) +
                                sizeof(ULONG64) - 1) / sizeof(ULONG64)];
                memset(buffer, 0, sizeof(buffer));

                // Initialize symbol information retrieval structures.
                DWORD64 sym_displacement = 0;
                PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(&buffer[0]);
                symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
                symbol->MaxNameLen = kMaxNameLength - 1;
                BOOL has_symbol = SymFromAddr(GetCurrentProcess(), frame, &sym_displacement, symbol);

                // Attempt to retrieve line number information.
                DWORD line_displacement = 0;
                IMAGEHLP_LINE64 line = {};
                line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
                BOOL has_line = SymGetLineFromAddr64(GetCurrentProcess(), frame, &line_displacement, &line);

                string str_frame;
                // Output the backtrace line.
                if (has_symbol) 
                {
                    str_frame += str(format("%1% [0x%2%+%3%]") % symbol->Name % trace[i] % sym_displacement);
                } 
                else 
                {
                    str_frame += str(format("(No symbol) [0x%1%]") % trace[i]);
                }

                if (has_line) 
                {
                    str_frame += str(format(" (%1%:%2%)") % ExtractFileName(line.FileName) % line.LineNumber);
                }

                stack->push_back(str_frame);
            }
        }

    private:
        SymbolContext() : init_error_(ERROR_SUCCESS) 
        {
            // Initializes the symbols for the process.
            // Defer symbol load until they're needed, use undecorated names, and
            // get line numbers.
            SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES);
            if (SymInitialize(GetCurrentProcess(), NULL, TRUE)) 
            {
                init_error_ = ERROR_SUCCESS;
            } else 
            {
                init_error_ = GetLastError();
                // TODO(awong): Handle error: SymInitialize can fail with
                // ERROR_INVALID_PARAMETER.
                // When it fails, we should not call debugbreak since it kills the current
                // process (prevents future tests from running or kills the browser
                // process).
                // DLOG(ERROR) << "SymInitialize failed: " << init_error_;
            }
        }

        DWORD init_error_;
        static Mutex lock_;
        static SymbolContext* g_instance_;
    };

    Mutex SymbolContext::lock_;
    SymbolContext* SymbolContext::g_instance_ = NULL;

    Backtrace::Backtrace(int exclude_cnt /* = DEFAULT_EXCLUDE_FIRST_FRAMES_COUNT */, int depth /* = DEFAULT_STACK_DEPTH */)
        : backtrace_(depth)
    {
        trace_size_ = CaptureStackBackTrace(exclude_cnt, depth, &backtrace_[0], NULL);
    }

    Backtrace::Stack Backtrace::GetStack() const
    {
        Backtrace::Stack result;
        SymbolContext* context = SymbolContext::GetInstance();
        if (context->init_error() == ERROR_SUCCESS) 
        {
            try
            {
                context->DumpTrace(&backtrace_[0], trace_size_, &result);
            }
            catch (boost::exception& e)
            {
                cout << endl << "--- Error dumping stack trace --- " << endl;
                cout << diagnostic_information(e) << endl << endl;
            }
            catch (exception& e)
            {
                cout << endl << "--- Error dumping stack trace --- " << endl;
                cout << e.what() << endl << endl;
            }
            catch (...)
            {
                cout << endl << "--- Error dumping stack trace --- " << endl;
                cout << "UNKNOWN EXCEPTION" << endl << endl;
            }
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

    Backtrace::Stack Backtrace::GetStack() const
    {
        Backtrace::Stack result;
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