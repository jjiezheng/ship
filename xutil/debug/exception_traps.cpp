#include "stdafx.h"
#include "exception_traps.h"
#include "untyped_exception.h"
#include "symbol_server.h"

#include "exception_formatter.h"

#include "../system/process.h"
#include "../threading.h"

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
                g__stack_trace_pointers_.reset(saved_info);
            }
            *saved_info = *ex_info;
            return EXCEPTION_CONTINUE_SEARCH;
        }

        static FatalErrorHandler xtrap_fatal_error_handler;

#ifdef WIN32
        const DWORD EXCEPTION_USER = 0xE06D7363U;

        LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* pExp)
        {
            BacktraceStack stack = SymbolServer::instance()->DumpContext(pExp->ContextRecord);
            string exception_details;
            
            UntypedException untyped(*pExp->ExceptionRecord);
            std::exception* std_error = untyped.exception_cast<std::exception>();
            if (std_error)
            {
                exception_details = format_exception(*std_error, false);
            }
            else
            {
                boost::exception* boost_error = untyped.exception_cast<boost::exception>();
                if (boost_error)
                {
                    exception_details = format_exception(*boost_error, false);
                }
            }
            
            LOG_FATAL(logs::app, "Unhandled exception" << endl << exception_details << "Stack trace:" << endl << stack);
            
            if (xtrap_fatal_error_handler)
            {
                xtrap_fatal_error_handler();
            }
            return EXCEPTION_CONTINUE_SEARCH;
        }

#endif
    }

    void RegisterExceptionTraps(FatalErrorHandler error_handler /* = NULL */)
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

        RegisterExceptionStackTraceTrap();
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

        BacktraceStack stack = SymbolServer::instance()->DumpContext(pExp->ContextRecord);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    void RegisterExceptionStackTraceTrap()
    {
        AddVectoredExceptionHandler(FALSE, StackTraceVectoredHandler);
    }

    PEXCEPTION_POINTERS GetExceptionStackTracePointers()
    {
        return g__stack_trace_pointers_.get();
    }

    PEXCEPTION_POINTERS GetExceptionStackTracePointers(const type_info& exception_type, const void* exception_object /* = NULL */)
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
        void* found_exception = untyped.FindException(exception_type);
        if (found_exception == NULL)
        {
            return NULL;
        }

        if (exception_object != NULL && found_exception != exception_object)
        {
            return NULL;
        }

        return ex_info;
    }
#endif

}