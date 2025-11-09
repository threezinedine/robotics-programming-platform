#include "modules/structures/project.h"
#include "core/json.h"

namespace rpp
{
    STRUCTURE_SAVE_LOAD_IMPLEMENT(Project);

    Project::Project()
        : m_name("UnnamedProject")
    {
        RPP_PROFILE_SCOPE();
    }

    Project::Project(const ProjectDescription &desc)
        : m_name(desc.name), m_functionNames(desc.functionNames)
    {
        RPP_PROFILE_SCOPE();
    }

    Project::Project(const Project &other)
        : m_name(other.m_name), m_functionNames(other.m_functionNames)
    {
        RPP_PROFILE_SCOPE();
    }

    Project::~Project()
    {
        RPP_PROFILE_SCOPE();
    }

    ProjectDescription Project::ToDescription() const
    {
        RPP_PROFILE_SCOPE();
        ProjectDescription desc;
        desc.name = m_name;
        desc.functionNames = m_functionNames;
        return desc;
    }

    void Project::AddNewFunction(const String &functionName)
    {
        RPP_PROFILE_SCOPE();
        String uniqueName = functionName;
        u32 functionsCount = m_functionNames.Size();

        while (TRUE)
        {
            b8 nameExists = FALSE;

            for (u32 functionIndex = 0; functionIndex < functionsCount; functionIndex++)
            {
                if (m_functionNames[functionIndex] == uniqueName)
                {
                    nameExists = TRUE;
                    break;
                }
            }

            if (!nameExists)
            {
                break;
            }

            uniqueName = Format("{} (Copy)", uniqueName);
        }

        m_functionNames.Push(uniqueName);
    }
}