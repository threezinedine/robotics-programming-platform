#include "modules/structures/function.h"
#include "modules/structures/project.h"

namespace rpp
{
    STRUCTURE_SAVE_LOAD_IMPLEMENT(Function);

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

    FunctionDescription Function::ToDescription() const
    {
        RPP_PROFILE_SCOPE();
        FunctionDescription desc;
        desc.name = m_name;
        return desc;
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
        m_addedFunctionIndex = m_pProject->GetFunctionDescriptions().Size() - 1;
    }

    void AddFunctionCommand::UndoImpl()
    {
        RPP_PROFILE_SCOPE();
        RPP_ASSERT(m_pProject != nullptr);
        RPP_ASSERT(m_addedFunctionIndex != INVALID_ID);
        RPP_ASSERT(m_addedFunctionIndex < m_pProject->GetFunctionDescriptions().Size());

        m_pProject->RemoveFunction(m_addedFunctionIndex);
    }

    DeleteFunctionCommand::DeleteFunctionCommand(Project *pProject, u32 functionIndex)
        : Command(),
          m_pProject(pProject),
          m_functionIndex(functionIndex)
    {
        RPP_PROFILE_SCOPE();
    }

    DeleteFunctionCommand::~DeleteFunctionCommand()
    {
        RPP_PROFILE_SCOPE();
    }

    b8 DeleteFunctionCommand::CanExecuteImpl() const
    {
        RPP_PROFILE_SCOPE();
        if (m_pProject == nullptr)
        {
            return FALSE;
        }

        const Array<FunctionDescription> &functionDescriptions = m_pProject->GetFunctionDescriptions();
        return m_functionIndex < functionDescriptions.Size();
    }

    void DeleteFunctionCommand::ExecuteImpl()
    {
        RPP_PROFILE_SCOPE();
        Array<FunctionDescription> &functionDescriptions = m_pProject->GetFunctionDescriptions();
        RPP_ASSERT(m_functionIndex < functionDescriptions.Size());

        functionDescriptions.Erase(m_functionIndex);
    }

    void DeleteFunctionCommand::UndoImpl()
    {
        RPP_PROFILE_SCOPE();
    }
} // namespace rpp
