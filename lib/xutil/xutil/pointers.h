#pragma once

namespace xutil
{
    template <typename T>
    unique_ptr<T> make_unique()
    {
        return unique_ptr<T>(new T());
    }

    template <typename T, typename A1>
    unique_ptr<T> make_unique(const A1& a1)
    {
        return unique_ptr<T>(new T(a1));
    }

    template <typename T, typename A1, typename A2>
    unique_ptr<T> make_unique(const A1& a1, const A2& a2)
    {
        return unique_ptr<T>(new T(a1, a2));
    }

    template <typename T, typename A1, typename A2, typename A3>
    unique_ptr<T> make_unique(const A1& a1, const A2& a2, const A3& a3)
    {
        return unique_ptr<T>(new T(a1, a2, a3));
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4>
    unique_ptr<T> make_unique(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
    {
        return unique_ptr<T>(new T(a1, a2, a3, a4));
    }
}