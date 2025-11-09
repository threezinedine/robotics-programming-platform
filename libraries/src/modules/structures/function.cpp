#include "modules/structures/function.h"
#include "modules/structures/project.h"

namespace rpp
{
    Function::Function()
        : m_name("UnnamedFunction")
    {
        RPP_PROFILE_SCOPE();
    }

    Function::Function(const FunctionDescription &desc)
        : m_name(desc.name)
    {
        RPP_PROFILE_SCOPE();
    }

    Function::Function(const Function &other)
        : m_name(other.m_name)
    {
        RPP_PROFILE_SCOPE();
    }

    Function::~Function()
    {
        RPP_PROFILE_SCOPE();
    }

    Function Function::CreateFunction(const FunctionDescription &desc)
    {
        RPP_PROFILE_SCOPE();
        return Function(desc);
    }

    AddFunctionCommand::AddFunctionCommand(Project *pProject)
        : Command(),
          m_pProject(pProject),
          m_functionDesc(),
          m_addedFunctionIndex(INVALID_ID)
    {
        RPP_PROFILE_SCOPE();
        m_functionDesc.name = "NewFunction";
    }

    AddFunctionCommand::AddFunctionCommand(Project *pProject, const FunctionDescription &desc)
        : Command(),
          m_pProject(pProject),
          m_functionDesc(desc),
          m_addedFunctionIndex(INVALID_ID)
    {
        RPP_PROFILE_SCOPE();
    }

    AddFunctionCommand::~AddFunctionCommand()
    {
        RPP_PROFILE_SCOPE();
    }

    b8 AddFunctionCommand::CanExecuteImpl() const
    {
        RPP_PROFILE_SCOPE();
        return m_pProject != nullptr;
    }

    void AddFunctionCommand::ExecuteImpl()
    {
        RPP_ASSERT(m_addedFunctionIndex == INVALID_ID);

        RPP_PROFILE_SCOPE();
        RPP_ASSERT(m_pProject != nullptr);
        m_pProject->AddNewFunction(m_functionDesc.name);
        m_addedFunctionIndex = m_pProject->GetFunctionNames().Size() - 1;
    }

    void AddFunctionCommand::UndoImpl()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(m_pProject != nullptr);
        RPP_ASSERT(m_addedFunctionIndex != INVALID_ID);
        RPP_ASSERT(m_addedFunctionIndex < m_pProject->GetFunctionNames().Size());
        Array<String> &functionNames = m_pProject->GetFunctionNames();
        functionNames.Erase(m_addedFunctionIndex);
    }
} // namespace rpp
