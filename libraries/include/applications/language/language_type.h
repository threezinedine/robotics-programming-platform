#pragma once
#include "modules/modules.h"

namespace rpp
{
    /**
     * All supported data types in the current language.
     */
    enum class DataType : u8
    {
        DATATYPE_NULL,
        DATATYPE_BOOL,
        DATATYPE_INT,
        DATATYPE_FLOAT,
        DATATYPE_STRING,

        DATATYPE_COUNT
    };
} // namespace rpp
