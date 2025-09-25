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
     * @brief Represents a project with a name.
     */
    class RPP_PYTHON_BINDING Project
    {
    public:
        Project();
        Project(const ProjectDescription &desc);
        Project(const Project &other);
        ~Project();

    public:
        static Project CreateProject(const String &descJson) RPP_PYTHON_BINDING;

    public:
        inline const String &GetName() RPP_PYTHON_BINDING;

    private:
        String m_name; ///< The name of the project.
    };

    const String &Project::GetName()
    {
        return m_name;
    }
} // namespace rpp
