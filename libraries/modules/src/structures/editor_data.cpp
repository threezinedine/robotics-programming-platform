#include "modules/structures/editor_data.h"

namespace rpp
{
    STRUCTURE_SAVE_LOAD_IMPLEMENT(EditorData);

    EditorData::EditorData()
    {
    }

    EditorData::EditorData(const EditorDataDescription &desc)
    {
        u32 recentCount = desc.recentProjects.Size();
        if (recentCount > m_recentProjects.Capacity())
        {
            m_recentProjects.Reallocate(recentCount);
        }

        for (u32 i = 0; i < recentCount; i++)
        {
            m_recentProjects.Push(desc.recentProjects[i]);
        }
    }

    EditorData::EditorData(const EditorData &other)
    {
        u32 recentCount = other.m_recentProjects.Size();
        if (recentCount > m_recentProjects.Capacity())
        {
            m_recentProjects.Reallocate(recentCount);
        }

        for (u32 i = 0; i < recentCount; i++)
        {
            m_recentProjects.Push(other.m_recentProjects[i]);
        }
    }

    EditorData::~EditorData()
    {
    }

    EditorDataDescription EditorData::ToDescription() const
    {
        EditorDataDescription desc;
        desc.recentProjects = m_recentProjects;
        return desc;
    }

    void EditorData::AddRecentProject(const String &projectPath)
    {
        m_recentProjects.Push(projectPath);
    }
} // namespace rpp