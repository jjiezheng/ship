#include "stdafx.h"
#include "untyped_exception.h"

namespace xutil
{
    UntypedException::UntypedException(const EXCEPTION_RECORD& er)
    {
         exception_object = reinterpret_cast<void*>(er.ExceptionInformation[1]);
         type_array = reinterpret_cast<_ThrowInfo*>(er.ExceptionInformation[2])->pCatchableTypeArray;
    }

    void* UntypedException::FindException(const type_info& ti) const
    {
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