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

        // Read functions from names
        u32 functionCount = m_functionNames.Size();
        Array<u32> missingFunctionIndices(functionCount);

        for (u32 functionIndex = 0u; functionIndex < functionCount; functionIndex++)
        {
            FunctionDescription functionDesc;
            b8 functionExists = FileSystem::PathExists(Format("files/{}.rppfunc", m_functionNames[functionIndex]));

            if (!functionExists)
            {
                missingFunctionIndices.Push(functionIndex);
                RPP_LOG_WARNING("Function file for 'files/{}.rppfunc' is missing, delete it from the project", m_functionNames[functionIndex]);
                continue;
            }

            FileHandle file = FileSystem::OpenFile(Format("files/{}.rppfunc", m_functionNames[functionIndex]), FILE_MODE_READ);
            String fileContent = FileSystem::Read(file);
            FileSystem::CloseFile(file);

            functionDesc = FromString<FunctionDescription>(fileContent);
            m_functionDescriptions.Push(functionDesc);
        }

        // Remove missing functions from names list
        for (i32 i = missingFunctionIndices.Size() - 1; i >= 0; i--)
        {
            u32 missingIndex = missingFunctionIndices[i];
            m_functionNames.Erase(missingIndex);
        }
    }

    Project::Project(const Project &other)
        : m_name(other.m_name), m_functionDescriptions(other.m_functionDescriptions)
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

        FunctionDescription functionDesc;
        functionDesc.name = uniqueName;

        m_functionNames.Push(uniqueName);
        m_functionDescriptions.Push(functionDesc);
    }

    void Project::RemoveFunction(u32 functionIndex)
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(functionIndex < m_functionDescriptions.Size());
        m_functionDescriptions.Erase(functionIndex);
        m_functionNames.Erase(functionIndex);

        FileSystem::DeleteFile(Format("files/{}.rppfunc", m_functionNames[functionIndex]));
    }
}