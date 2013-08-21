#include "stdafx.h"
#include "var_table.h"

namespace ship
{
    VarTable::~VarTable()
    {
    }

    string VarTable::Expand(const string& value) const
    {
        return ExpandKey('$', value);
    }
}