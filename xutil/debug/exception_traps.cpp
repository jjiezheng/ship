#include "stdafx.h"
#include "exception_traps.h"
#include "untyped_exception.h"
#include "../system/process.h"
#include "../threading.h"

#ifdef WIN32
#include "stack_walker.h"
#endif

namespace xutil
{
    bool xtrap_retrhrow_exceptions;

    namespace
    {
        thread_specific_ptr<EXCEPTION_POINTERS> g__stack_trace_pointers_;

        LONG CALLBACK StackTraceVectoredHandler(PEXCEPTION_POINTERS ex_info)
        {
            PEXCEPTION_POINTERS saved_info = g__stack_trace_pointers_.get();
            if (!saved_info)
            {
                saved_info = new EXCEPTION_POINTERS();
            }
            *saved_info = *ex_info;
            return EXCEPTION_CONTINUE_SEARCH;
        }

        static FatalErrorHandler xtrap_fatal_error_handler;

#ifdef WIN32
        const DWORD EXCEPTION_USER = 0xE06D7363U;

        thread_specific_ptr<StackWalker> g_stack_walker;
        thread_specific_ptr<string> g_seh_info;

        void SaveSEH_Info(const char* msg)
        {
            string* info = g_seh_info.get();
            if (!info)
            {
                info = new string();
                g_seh_info.reset(info);
            }
            info->append(msg);
        }

        StackWalker* GetThreadStackWalker()
        {
            StackWalker* walker = g_stack_walker.get();
            if (!walker)
            {
                walker = new StackWalker();
                walker->SetOutputProc(SaveSEH_Info);
                g_stack_walker.reset(walker);
            }

            return walker;
        }

        void ResetThreadExceptionInfo()
        {
            string* info = g_seh_info.get();
            if (!info)
            {
                info = new string();
                g_seh_info.reset(info);
            }
            
            info->clear();
        }

        void DumpThreadStack(HANDLE hThread, const CONTEXT* context)
        {
            ResetThreadExceptionInfo();

            StackWalker* walker = GetThreadStackWalker();
            walker->ShowCallstack(hThread, context);
        }

        LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* pExp)
        {
            DumpThreadStack(GetCurrentThread(), pExp->ContextRecord);
            //ITS_FATAL(logs::app, "Unhandled exception" << endl << GetSEH_Info());
            
            if (xtrap_fatal_error_handler)
            {
                xtrap_fatal_error_handler();
            }
            return EXCEPTION_CONTINUE_SEARCH;
        }

#endif
    }

    void RegisterExceptionTraps(FatalErrorHandler error_handler /* = nullptr */)
    {
#ifdef WIN32
        if (IsDebuggerPresent())
        {
            xtrap_retrhrow_exceptions = true;
        }

        if (!xtrap_retrhrow_exceptions)
        {
            ::SetUnhandledExceptionFilter(&UnhandledExceptionHandler);
        }
#endif

        xtrap_fatal_error_handler  = error_handler;
    }


#ifdef WIN32
    LONG WINAPI SEH_Handler(EXCEPTION_POINTERS* pExp, DWORD dwExpCode)
    {
        if (dwExpCode == EXCEPTION_USER || xtrap_retrhrow_exceptions)
        {
            return EXCEPTION_CONTINUE_SEARCH;
        }

        DumpThreadStack(GetCurrentThread(), pExp->ContextRecord);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    string GetSEH_Info()
    {
        string* seh_message = g_seh_info.get();
        if (seh_message)
        {
            return *seh_message;
        }
        else
        {
            return "no exception information";
        }
    }

    void RegisterExceptionStackTraceTrap()
    {
        AddVectoredExceptionHandler(TRUE, StackTraceVectoredHandler);
    }

    PEXCEPTION_POINTERS GetExceptionStackTracePointers()
    {
        return g__stack_trace_pointers_.get();
    }

    PEXCEPTION_POINTERS GetExceptionStackTracePointers(void* exception_object)
    {
        PEXCEPTION_POINTERS ex_info = GetExceptionStackTracePointers();
        if (!ex_info)
        {
            return NULL;
        }

        const EXCEPTION_RECORD& er = *ex_info->ExceptionRecord;
        if (er.ExceptionCode != EXCEPTION_USER)
        {
            return NULL;
        }

        UntypedException untyped(er);
        return NULL;
    }

    //void LogSEH_Info(Logger& logger, const char* message, const char* tag)
    //{
    //     ITS_FATAL(logger, message << " '" << tag << "' - " << "SEH exception" << endl <<
    //                       "  Stack trace:" << endl << GetSEH_Info());
    //}
#endif

}