#include "modules/history/command.h"

namespace rpp
{
    Command::Command()
    {
        RPP_PROFILE_SCOPE();
    }

    Command::~Command()
    {
        RPP_PROFILE_SCOPE();
    }

    b8 Command::CanExecute() const
    {
        RPP_PROFILE_SCOPE();
        return CanExecuteImpl();
    }

    void Command::Execute()
    {
        RPP_PROFILE_SCOPE();
        ExecuteImpl();
    }

    void Command::Undo()
    {
        RPP_PROFILE_SCOPE();
        UndoImpl();
    }
} // namespace rpp
