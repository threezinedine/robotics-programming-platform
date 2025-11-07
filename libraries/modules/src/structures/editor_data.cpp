#include "modules/structures/editor_data.h"

namespace rpp
{
    STRUCTURE_SAVE_LOAD_IMPLEMENT(EditorData);

    EditorData::EditorData()
    {
    }

    EditorData::EditorData(const EditorDataDescription &desc)
    {
        m_recentProjects = desc.recentProjects;
    }

    EditorData::EditorData(const EditorData &other)
    {
        m_recentProjects = other.m_recentProjects;
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