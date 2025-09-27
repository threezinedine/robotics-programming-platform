#pragma once
#include "core/core.h"

namespace rpp
{
    /**
     * @brief The storage data of a `function` in the system. This is used in Python binding and JSON mapping.
     */
    struct RPP_JSON RPP_PYTHON_BINDING FunctionDescription
    {
        String name RPP_JSON_KEY("name"); ///< The name of the function.
    };

    /**
     * @brief The main wrapper beside `class` for representing the functional of the system. This is used in Python binding and JSON mapping.
     */
    class RPP_PYTHON_BINDING Function
    {
    public:
        Function();
        Function(const FunctionDescription &desc);
        Function(const Function &other);
        ~Function();

    private:
        String m_name; ///< The name of the function.
    };
} // namespace rpp
