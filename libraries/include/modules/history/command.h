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

        /**
         * @brief Gets the name of the command.
         *
         * @return The name of the command.
         */
        virtual String Name() const = 0;

        /**
         * @brief The tag is the way to categorize commands into groups.
         *
         * @return The tag of the command.
         */
        virtual u32 Tag() const = 0;

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

#define RPP_DEFINE_COMMAND(className, classTag)                        \
public:                                                                \
    inline String Name() const override { return String(#className); } \
    inline u32 Tag() const override { return classTag; }
