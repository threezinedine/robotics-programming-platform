#pragma once
#include "core/core.h"

namespace rpp
{
    class Command;

    /**
     * Called each time a command is executed.
     *
     * @param pCommand The command that was executed.
     */
    using HistoryCommandExecuteCallback = std::function<void(Command *)>;

    /**
     * Called each time a command is undone.
     *
     * @param pCommand The command that was undone.
     */
    using HistoryCommandUndoCallback = std::function<void(Command *)>;

    /**
     * Called when the history becomes empty.
     */
    using HistoryCommandEmptyCallback = std::function<void()>;

    /**
     * The history manager which manages the command history for undo and redo operations.
     */
    class RPP_SINGLETON HistoryManager
    {
        RPP_SINGLETON_DEFINE(HistoryManager);

    public:
        /**
         * Used to run a command and add it to the history.
         */
        void ExecuteCommand(Command *pCommand);

        /**
         * Undoes the last executed command. If there are no commands to undo, this function does nothing.
         */
        void Undo();

        /**
         * Sets the callback to be called when a command is executed.
         */
        inline b8 Empty() const { return m_commands.Size() == 0; };

        /**
         * Gets the number of commands in the history.
         */
        inline u32 Count() const { return m_commands.Size(); };

    private:
        Stack<Command *> m_commands; /// The list of executed commands.

        HistoryCommandExecuteCallback m_onCommandExecute; /// Called when a command is executed.
        HistoryCommandUndoCallback m_onCommandUndo;       /// Called when a command is undone.
        HistoryCommandEmptyCallback m_onHistoryEmpty;     /// Called when the history becomes empty.
    };
}