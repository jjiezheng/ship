#pragma once

#include <exception>
#include <boost/exception/all.hpp>

#include "debug/backtrace.h"
#include "debug/exception_traps.h"

#define XTHROW BOOST_THROW_EXCEPTION
#define XTHROW_EXCEPTION(x) XTHROW(xutil::Exception(x))

#define XUTIL_DECLARE_ERROR_INFO(InfoType, ValueType) \
    typedef boost::error_info<struct InfoType##_, ValueType> InfoType;

namespace xutil
{
    typedef boost::errinfo_errno ErrorInfo_SystemErrorCode;
    XUTIL_DECLARE_ERROR_INFO(ErrorInfo_Message, string);
    XUTIL_DECLARE_ERROR_INFO(ErrorInfo_Backtrace, Backtrace);

    class Exception : public std::exception, public boost::exception
    {
    public:
        static const int backtrace_skip;

        Exception();
        Exception(const string& message);
        Exception(const format& format);

        virtual ~Exception() throw();

        template <typename T>
        const typename T::value_type* get() const
        {
            return boost::get_error_info<T>(*this);
        }

        template <typename T>
        const typename T::value_type try_get(const typename T::value_type& default_value) const
        {
            typedef typename T::value_type TValue;
            const TValue* p_value = get<T>();
            return p_value ? *p_value : default_value;
        }

        const int* system_errno() const { return get<ErrorInfo_SystemErrorCode>(); }
        const Backtrace* trace() const {  return get<ErrorInfo_Backtrace>();  }
        string message() const {  return try_get<ErrorInfo_Message>("");  }

        virtual const char* what() const throw();
        virtual const char* detailed_info() const throw();

    private:
        mutable string exception_str_;
    };
}
