#pragma once
#include "core/core.h"
#include "modules/history/history.h"
#include "tag.h"
#include "utils.h"

namespace rpp
{
    class Project;

    /**
     * @brief The storage data of a `function` in the system. This is used in Python binding and JSON mapping.
     */
    struct RPP_JSON FunctionDescription
    {
        String name RPP_JSON_KEY("name"); ///< The name of the function.
    };

    /**
     * @brief The main wrapper beside `class` for representing the functional of the system. Each function can be compiled into bytecode and run on the MCU runtime.
     *   This object is used in Python binding and JSON mapping and the graphical runtime environment (the MCU runtime only receives the bytecode).
     */
    class Function
    {
        STRUCTURE_SAVE_LOAD_DEFINE(Function);

    public:
        /**
         * @brief Get the name of the function.
         */
        inline const String &GetName() const;

    private:
        String m_name; ///< The name of the function.
    };

    const String &Function::GetName() const
    {
        return m_name;
    }

    class AddFunctionCommand : public Command
    {
        RPP_DEFINE_COMMAND(AddFunctionCommand, FUNCTION_COMMAND_TAG);

    public:
        AddFunctionCommand(Project *pProject);
        AddFunctionCommand(Project *pProject, const FunctionDescription &desc);
        ~AddFunctionCommand();

    protected:
        b8 CanExecuteImpl() const;
        void ExecuteImpl() override;
        void UndoImpl() override;

    private:
        Project *m_pProject;                ///< The project to which the function will be added.
        FunctionDescription m_functionDesc; ///< The description of the function to be added.
        u32 m_addedFunctionIndex;           ///< The index of the added function in the project's function list.
    };

    class DeleteFunctionCommand : public Command
    {
        RPP_DEFINE_COMMAND(DeleteFunctionCommand, FUNCTION_COMMAND_TAG);

    public:
        DeleteFunctionCommand(Project *pProject, u32 functionIndex);
        ~DeleteFunctionCommand();

    protected:
        b8 CanExecuteImpl() const;
        void ExecuteImpl() override;
        void UndoImpl() override;

    private:
        Project *m_pProject; ///< The project from which the function will be deleted.
        u32 m_functionIndex; ///< The index of the function to be deleted.
    };
} // namespace rpp
