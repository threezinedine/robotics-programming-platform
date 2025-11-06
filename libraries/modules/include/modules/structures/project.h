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

    private:
        String m_name; ///< The name of the project.
    };

    const String &Project::GetName()
    {
        return m_name;
    }
} // namespace rpp
