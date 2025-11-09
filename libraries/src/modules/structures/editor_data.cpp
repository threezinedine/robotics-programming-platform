#include "modules/structures/editor_data.h"

namespace rpp
{
    STRUCTURE_SAVE_LOAD_IMPLEMENT(EditorData);

    EditorData::EditorData()
    {
        RPP_PROFILE_SCOPE();
    }

    EditorData::EditorData(const EditorDataDescription &desc)
    {
        RPP_PROFILE_SCOPE();
        m_recentProjects = desc.recentProjects;
    }

    EditorData::EditorData(const EditorData &other)
    {
        RPP_PROFILE_SCOPE();
        m_recentProjects = other.m_recentProjects;
    }

    EditorData::~EditorData()
    {
        RPP_PROFILE_SCOPE();
    }

    EditorDataDescription EditorData::ToDescription() const
    {
        RPP_PROFILE_SCOPE();
        EditorDataDescription desc;
        desc.recentProjects = m_recentProjects;
        return desc;
    }

    void EditorData::AddRecentProject(const String &projectPath)
    {
        RPP_PROFILE_SCOPE();
        u32 existingIndex = static_cast<u32>(-1);
        for (u32 i = 0; i < m_recentProjects.Size(); i++)
        {
            if (m_recentProjects[i] == projectPath)
            {
                existingIndex = i;
                break;
            }
        }

        if (existingIndex != static_cast<u32>(-1))
        {
            m_recentProjects.Erase(existingIndex);
        }

        m_recentProjects.Push(projectPath, 0);
    }
} // namespace rpp