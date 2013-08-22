#pragma once

namespace xutil
{
    class Exception;

    string format_exception(const std::exception& e);
    string format_exception(const boost::exception& e);
    string format_exception(const Exception& e);

    string format_boost_exception_details(const string& boost_details);
}