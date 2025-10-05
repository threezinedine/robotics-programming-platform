#include "applications/test_system/command_stack.h"

namespace rpp
{
    CommandStack::CommandStack()
    {
    }

    CommandStack::~CommandStack()
    {
    }

    void CommandStack::Push(Command command)
    {
        m_commands.Push(command);
    }

    Command CommandStack::Pop()
    {
        u32 count = m_commands.Size();
        RPP_ASSERT(count > 0);
        Command commandValue = m_commands[count - 1];
        m_commands.Erase(count - 1);
        return commandValue;
    }
} // namespace rpp
