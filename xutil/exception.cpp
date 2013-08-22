#include "stdafx.h"
#include "exception.h"

namespace xutil
{
    const int Exception::backtrace_skip = 2;

    Exception::Exception()
    {
        *this << ErrorInfo_Backtrace(Backtrace(backtrace_skip));
    }

    Exception::Exception(const string& message)
    {
        *this << ErrorInfo_Backtrace(Backtrace(backtrace_skip));
        *this << ErrorInfo_Message(message);
    }

    Exception::Exception(const format& format)
    {
        *this << ErrorInfo_Backtrace(Backtrace(backtrace_skip));
        *this << ErrorInfo_Message(str(format));
    }

    Exception::~Exception() throw()
    {
    }

    char const* Exception::what() const throw()
    {
        exception_str_ = format_boost_exception_details(detailed_info());
        return exception_str_.c_str();
    }

    const char* Exception::detailed_info() const throw()
    {
        return diagnostic_information_what(*this);
    }
}