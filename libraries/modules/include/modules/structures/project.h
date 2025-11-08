#pragma once
#include "core/core.h"
#include "utils.h"

namespace rpp
{
    /**
     * @brief The needed information which are used for creating a project object. This is used in Python binding and JSON mapping.
     */
    struct RPP_JSON ProjectDescription
    {
        String name RPP_JSON_KEY("name"); ///< The name of the project.
        Array<String> functionNames RPP_JSON_KEY("functionNames"); ///< The list of function names in the project.
    };

    /**
     * @brief Main object which is used for interacting with project (a workspace for functions, and classes, emulators). This object is
     *      used in Python binding and JSON mapping and the graphical runtime environment (the MCU runtime only receives the bytecode).
     */
    class Project
    {
        STRUCTURE_SAVE_LOAD_DEFINE(Project);

    public:
        /**
         * @brief Get the name of the project.
         */
        inline const String &GetName();

        inline const Array<String> &GetFunctionNames() const { return m_functionNames; }
        inline Array<String> &GetFunctionNames() { return m_functionNames; }

    public:
        /**
         * @brief Add a new function to the project with an unique name.
         * 
         * @param functionName The name of the new function. Default is "NewFunction".
         */
        void AddNewFunction(const String& functionName = "NewFunction");

    private:
        String m_name; ///< The name of the project.
        Array<String> m_functionNames; ///< The list of function names in the project.
    };

    const String &Project::GetName()
    {
        return m_name;
    }
} // namespace rpp
