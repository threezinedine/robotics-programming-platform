#pragma once
#include "core/core.h"

namespace rpp
{
    /**
     * The interface for each command which can be used in the history system.
     */
    class Command
    {
    public:
        /**
         * @brief Constructs a new Command object.
         */
        Command();

        /**
         * @brief Destroys the Command object.
         */
        virtual ~Command();

        /**
         * Checking by the history manager so that it can know whether the command can be executed.
         *
         * @return True if the command can be executed, false otherwise (the command will be ignored).
         */
        b8 CanExecute() const;

        /**
         * Executes the command.
         */
        void Execute();

        /**
         * Undoes the command.
         */
        void Undo();

    protected:
        /**
         * Implement this method to define whether the command can be executed.
         */
        virtual b8 CanExecuteImpl() const { return TRUE; }

        /**
         * Implement this method to define the command execution logic.
         */
        virtual void ExecuteImpl() {}

        /**
         * Implement this method to define the command undo logic.
         */
        virtual void UndoImpl() {}
    };
} // namespace rpp
