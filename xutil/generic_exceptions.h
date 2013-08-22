#pragma once

#include "exception.h"

#define XTHROW_NOT_IMPLEMENTED() XTHROW(xutil::NotImplementedException())
#define XTHROW_NOT_SUPPORTED(msg) XTHROW(xutil::NotSupportedException(msg))

namespace xutil
{
    class NotImplementedException : public Exception
    {
    public:
        NotImplementedException();
        NotImplementedException(const string& message);
        NotImplementedException(const format& format);

        virtual ~NotImplementedException() throw();
    };

    class NotSupportedException : public Exception
    {
    public:
        NotSupportedException();
        NotSupportedException(const string& message);
        NotSupportedException(const format& format);

        virtual ~NotSupportedException() throw();
    };

    class SystemError : public Exception
    {
    public:
        static int GetSystemErrorCode();

        SystemError();
        SystemError(const string& message);
        SystemError(const format& format);

        virtual ~SystemError() throw();
    };
}
