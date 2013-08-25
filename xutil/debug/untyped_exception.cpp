#include "stdafx.h"
#include "untyped_exception.h"

namespace xutil
{
    static const DWORD EXCEPTION_USER = 0xE06D7363U;

    UntypedException::UntypedException(const EXCEPTION_RECORD& er)
    {
         if (er.ExceptionCode == EXCEPTION_USER)
         {
             exception_object = reinterpret_cast<void*>(er.ExceptionInformation[1]);
             type_array = reinterpret_cast<_ThrowInfo*>(er.ExceptionInformation[2])->pCatchableTypeArray;
         }
         else
         {
             exception_object = NULL;
             type_array = NULL;
         }
    }

    void* UntypedException::FindException(const type_info& ti) const
    {
        if (!exception_object || !type_array)
        {
            return NULL;
        }

        for (int i = 0; i < type_array->nCatchableTypes; ++i) 
        {
            _CatchableType& type_i = *type_array->arrayOfCatchableTypes[i];
            const std::type_info& ti_i = *reinterpret_cast<std::type_info*>(type_i.pType);
            if (ti_i == ti) 
            {
                char* base_address = reinterpret_cast<char*>(exception_object);
                base_address += type_i.thisDisplacement.mdisp;
                return base_address;
            }
        }
        return 0;
    }
}