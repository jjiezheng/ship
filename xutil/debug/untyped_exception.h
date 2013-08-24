#pragma once

namespace xutil
{
    class UntypedException 
    {
    public:
        UntypedException(const EXCEPTION_RECORD& er);

        void* exception_object;
        _CatchableTypeArray* type_array;

        template <typename T>
        T* exception_cast() const
        {
            const type_info& ti = typeid(T);
            return reinterpret_cast<T *>(FindException(ti));
        }

    private:
        void* FindException(const type_info& ti) const;
    };
}