#include "modules/structures/editor_data.h"

namespace rpp
{
    // STRUCTURE_SAVE_LOAD_IMPLEMENT(EditorData);
    EditorData *EditorData::Create() { return new (Allocate(sizeof(EditorData), "C:\\Users\\Acer\\Project\\robotics-programming-platform\\libraries\\modules\\src\\structures\\editor_data.cpp", 5)) EditorData(); }
    EditorData *EditorData::Create(const EditorDataDescription &desc) { return new (Allocate(sizeof(EditorData), "C:\\Users\\Acer\\Project\\robotics-programming-platform\\libraries\\modules\\src\\structures\\editor_data.cpp", 5)) EditorData(desc); }
    EditorData *EditorData::Create(const String &filePath)
    {
        if (!(FileSystem::PathExists(filePath)))
        {
            rpp::Logging::GetInstance()->Log(rpp::LogLevel::ERROR, Format("Assertion failed: "
                                                                          "FileSystem::PathExists(filePath)"),
                                             "C:\\Users\\Acer\\Project\\robotics-programming-platform\\libraries\\modules\\src\\structures\\editor_data.cpp", 5);
            __debugbreak();
        };
        FileHandle file = FileSystem::OpenFile(filePath, u32(0x00));
        String fileContent = FileSystem::Read(file);
        FileSystem::CloseFile(file);
        auto des = FromString<EditorDataDescription>(fileContent);
        auto res = new (Allocate(sizeof(EditorData), "C:\\Users\\Acer\\Project\\robotics-programming-platform\\libraries\\modules\\src\\structures\\editor_data.cpp", 5)) EditorData(des);
        return res;
    }

    void EditorData::Save(const String &filePath) const
    {
        FileHandle file = FileSystem::OpenFile(filePath, u32(0x01));
        FileSystem::Write(file, ToString(this->ToDescription()));
        FileSystem::CloseFile(file);
    }

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