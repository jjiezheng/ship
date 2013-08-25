#pragma once

#include "exception_formatter.h"

#ifdef WIN32
    #define XTRY_CRITICAL __try
    #define XCATCH_CRITICAL(log, message, tag)                                    \
        __except(its::SEH_Handler(GetExceptionInformation(), GetExceptionCode())) \
        {                                                                         \
            LogSEH_Info(log, message, tag);                                       \
        }
#else
    #define XTRY_CRITICAL
    #define XCATCH_CRITICAL(log, message, tag)
    #define XCATCH_CRITICAL_ACTION(log, message, tag, action)
#endif

#define XCATCH_THREAD_INTERRUPT(log, message)     \
    catch (boost::thread_interrupted&)            \
    {                                             \
        ITS_DEBUG(log, message);                  \
        throw;                                    \
    }

#define XCATCH_EXCEPTION(log, message)                            \
    catch (boost::exception& e)                                   \
    {                                                             \
        ITS_ERROR(log, message << " - " << format_exception(e));  \
        if (its::Config::rethrow_exceptions)                      \
        {                                                         \
            throw;                                                \
        }                                                         \
    }                                                             \
    catch (std::exception& e)                                     \
    {                                                             \
        ITS_ERROR(log, message << " - " << format_exception(e));  \
        if (its::Config::rethrow_exceptions)                      \
        {                                                         \
            throw;                                                \
        }                                                         \
    }                                                             \
    catch (...)                                                   \
    {                                                             \
        ITS_ERROR(log, message << " -  Unkown error");            \
        if (its::Config::rethrow_exceptions)                      \
        {                                                         \
            throw;                                                \
        }                                                         \
    }

#define XCATCH_EXCEPTION_RETHROW(log, message)                    \
    catch (std::exception& e)                                     \
    {                                                             \
        ITS_ERROR(log, message << " - " << format_exception(e));  \
        throw;                                                    \
    }                                                             \
    catch (boost::exception& e)                                   \
    {                                                             \
        ITS_ERROR(log, message << " - " << format_exception(e));  \
        throw;                                                    \
    }                                                             \
    catch (...)                                                   \
    {                                                             \
        ITS_ERROR(log, message << " -  Unkown error");            \
        throw;                                                    \
    }



namespace xutil
{
    extern bool xtrap_retrhrow_exceptions;

    typedef void (*FatalErrorHandler)(void);
    void RegisterExceptionTraps(FatalErrorHandler error_handler = NULL);

    void RegisterExceptionStackTraceTrap();
    PEXCEPTION_POINTERS GetExceptionStackTracePointers();
    PEXCEPTION_POINTERS GetExceptionStackTracePointers(const type_info& exception_type, const void* exception_object = NULL);

    template <typename T>
    PEXCEPTION_POINTERS GetExceptionStackTracePointers(const T& exception_object)
    {
        const type_info& type = typeid(exception_object);
        return GetExceptionStackTracePointers(type, &exception_object);
    }

#ifdef WIN32
    LONG WINAPI SEH_Handler(EXCEPTION_POINTERS* pExp, DWORD dwExpCode);
    //void LogSEH_Info(Logger& logger, const char* message, const char* tag);
#endif
}