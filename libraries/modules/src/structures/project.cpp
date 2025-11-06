#include "modules/structures/project.h"
#include "core/json.h"

namespace rpp
{
    STRUCTURE_SAVE_LOAD_IMPLEMENT(Project);

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

    ProjectDescription Project::ToDescription() const
    {
        ProjectDescription desc;
        desc.name = m_name;
        return desc;
    }
}