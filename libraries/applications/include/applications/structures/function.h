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
     * @brief The main wrapper beside `class` for representing the functional of the system. Each function can be compiled into bytecode and run on the MCU runtime.
     *   This object is used in Python binding and JSON mapping and the graphical runtime environment (the MCU runtime only receives the bytecode).
     */
    class RPP_PYTHON_BINDING Function
    {
    public:
        Function();
        Function(const FunctionDescription &desc);
        Function(const Function &other);
        ~Function();

    public:
        /// @brief Create a function from the given description.
        /// @param desc The description of the function.
        /// @return The created function.
        static Function CreateFunction(const FunctionDescription &desc) RPP_PYTHON_BINDING;

    public:
        /**
         * @brief Get the name of the function.
         */
        inline const String &GetName() RPP_PYTHON_BINDING;

    private:
        String m_name; ///< The name of the function.
    };

    const String &Function::GetName()
    {
        return m_name;
    }
} // namespace rpp
