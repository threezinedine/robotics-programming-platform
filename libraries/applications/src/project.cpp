#include "applications/structures/project.h"
#include "core/json.h"

namespace rpp
{
    Project::Project()
        : m_name("UnnamedProject")
    {
    }

    Project::Project(const ProjectDescription &desc)
        : m_name(desc.name)
    {
    }

    Project::Project(const Project &other)
        : m_name(other.m_name)
    {
    }

    Project::~Project()
    {
    }

    Project Project::CreateProject(const ProjectDescription &desc) RPP_PYTHON_BINDING
    {
        return Project(desc);
    }
}