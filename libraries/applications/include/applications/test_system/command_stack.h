#pragma once
#include "modules/modules.h"

namespace rpp
{
    enum class Command : u32
    {
        COMMAND_WAIT,
    };

    class RPP_E2E_BINDING CommandStack
    {
    public:
        CommandStack();
        ~CommandStack();

        void Push(Command command) RPP_E2E_BINDING;

        Command Pop() RPP_E2E_BINDING;

        inline b8 Empty() const { return m_commands.Size() == 0; }

    private:
        Array<Command> m_commands;
    };
} // namespace rpp
