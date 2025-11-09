#include "modules/history/manager.h"
#include "modules/history/command.h"

namespace rpp
{
    RPP_SINGLETON_IMPLEMENT(HistoryManager);

    HistoryManager::HistoryManager()
        : m_commands(),
          m_onCommandExecute(nullptr),
          m_onCommandUndo(nullptr),
          m_onHistoryEmpty(nullptr)
    {
    }

    HistoryManager::~HistoryManager()
    {
    }

    void HistoryManager::ExecuteCommand(Command *pCommand)
    {
        if (pCommand == nullptr)
        {
            RPP_LOG_WARNING("HistoryManager::ExecuteCommand: Attempted to execute a null command.");
            return;
        }

        if (!pCommand->CanExecute())
        {
            RPP_LOG_WARNING("HistoryManager::ExecuteCommand: Command cannot be executed and will be ignored.");
            return;
        }

        pCommand->Execute();
        m_commands.Push(pCommand);

        if (m_onCommandExecute)
        {
            m_onCommandExecute(pCommand);
        }
    }

    void HistoryManager::Undo()
    {
        if (m_commands.Empty())
        {
            RPP_LOG_WARNING("HistoryManager::Undo: No commands to undo.");
            return;
        }

        Command *pCommand = m_commands.Top();
        m_commands.Pop();

        pCommand->Undo();

        if (m_onCommandUndo)
        {
            m_onCommandUndo(pCommand);
        }

        if (m_commands.Empty() && m_onHistoryEmpty)
        {
            m_onHistoryEmpty();
        }
        RPP_DELETE(pCommand);
    }
} // namespace rpp
