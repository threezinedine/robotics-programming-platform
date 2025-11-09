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
         * Resets the history, clearing all executed commands.
         */
        void Reset();

        /**
         * Sets the callback to be called when a command is executed.
         */
        inline b8 Empty() const { return m_commands.Size() == 0; };

        /**
         * Gets the number of commands in the history.
         */
        inline u32 Count() const { return m_commands.Size(); };

    public:
        /**
         * Sets the callback to be called when a command is executed.
         *
         * @param callback The callback function.
         */
        void SetOnCommandExecuteCallback(const HistoryCommandExecuteCallback &callback) { m_onCommandExecute = callback; }

        /**
         * Sets the callback to be called when a command is undone.
         *
         * @param callback The callback function.
         */
        void SetOnCommandUndoCallback(const HistoryCommandUndoCallback &callback) { m_onCommandUndo = callback; }

        /**
         * Sets the callback to be called when the history becomes empty.
         *
         * @param callback The callback function.
         */
        void SetOnHistoryEmptyCallback(const HistoryCommandEmptyCallback &callback) { m_onHistoryEmpty = callback; }

    private:
        Stack<Command *> m_commands; /// The list of executed commands.

        HistoryCommandExecuteCallback m_onCommandExecute; /// Called when a command is executed.
        HistoryCommandUndoCallback m_onCommandUndo;       /// Called when a command is undone.
        HistoryCommandEmptyCallback m_onHistoryEmpty;     /// Called when the history becomes empty.
    };
}