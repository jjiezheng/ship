#include "stdafx.h"
#include "generic_exceptions.h"

namespace xutil
{
    NotImplementedException::NotImplementedException()
    {
    }

    NotImplementedException::NotImplementedException(const string& message)
        : Exception(message)
    {
    }

    NotImplementedException::NotImplementedException(const format& format)
        : Exception(format)
    {
    }

    NotImplementedException::~NotImplementedException() throw()
    {
    }


    NotSupportedException::NotSupportedException()
    {
    }

    NotSupportedException::NotSupportedException(const string& message)
        : Exception(message)
    {
    }

    NotSupportedException::NotSupportedException(const format& format)
        : Exception(format)
    {
    }

    NotSupportedException::~NotSupportedException() throw()
    {
    }


    int SystemError::GetSystemErrorCode()
    {
        return GetLastError();
    }

    SystemError::SystemError()
    {
        *this << ErrorInfo_SystemErrorCode(GetSystemErrorCode());
    }

    SystemError::SystemError(const string& message)
        : Exception(message)
    {
        *this << ErrorInfo_SystemErrorCode(GetSystemErrorCode());
    }

    SystemError::SystemError(const format& format)
        : Exception(format)
    {
        *this << ErrorInfo_SystemErrorCode(GetSystemErrorCode());
    }

    SystemError::~SystemError() throw()
    {
    }
}

