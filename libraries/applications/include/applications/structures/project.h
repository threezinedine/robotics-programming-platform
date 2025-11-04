#pragma once
#include "core/core.h"

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
    public:
        Project();
        Project(const ProjectDescription &desc);
        Project(const Project &other);
        ~Project();

    public:
        /// @brief Create a project from the given description.
        /// @param desc The description of the project.
        /// @return The created project pointer.
        static Project *CreateProject(const ProjectDescription &desc);

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
