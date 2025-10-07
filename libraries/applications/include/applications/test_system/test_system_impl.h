#pragma once
#include "modules/modules.h"
#include "command_stack.h"

namespace rpp
{
    /**
     * Used for e2e testing of the applications module
     */
    class RPP_SINGLETON TestSystem
    {
        RPP_SINGLETON_DEFINE(TestSystem);

    public:
        /**
         * Starting the test system with the given scripts (these scripts is python scripts).
         *
         * @param resultFilePath The path to the result file where test results will be written (json format). This parameter is required.
         * @param setupFilePath The path to the setup script (optional).
         * @param updateFilePath The path to the update script (optional).
         * @param shutdownFilePath The path to the shutdown script (optional).
         *
         * @note these scripts' paths must be ABSOLUTE physical paths, so inside this module, we dont use the `FileSystem` module
         * to read these files, instead, we use the native file reading APIs.
         */
        void Initialize(const String &resultFilePath,
                        const String &setupFilePath = "",
                        const String &updateFilePath = "",
                        const String &shutdownFilePath = "");

        /**
         * Shuts down the test system, releasing all resources.
         */
        void Shutdown();

    public:
        /**
         * Updates the test system by executing the update script if provided.
         *
         * @param deltaTime The time elapsed since the last update call.
         *
         * @note this function will be called from the main thread, with locking mechanism for coroutine behavior
         */
        void Update(f32 deltaTime);

        /**
         * @brief Checks if the test system should close, typically when all tests are done.
         * @return TRUE if the test system should close, FALSE otherwise.
         */
        b8 ShouldApplicationClose();

        inline const CommandStack &GetCommandStack() const { return m_commandStack; }
        inline CommandStack &GetCommandStack() { return m_commandStack; }

    public:
        /**
         * @brief Yields the execution of the current thread, allowing other threads to run.
         * @note This function is useful for implementing coroutine-like behavior in the test system. Only
         * be called from the test thread.
         */
        void Yield() RPP_E2E_BINDING;

        /**
         * @brief Pauses the execution of the current thread for a specified duration in milliseconds.
         * @param milliseconds The duration to wait in milliseconds.
         * @note This function can be called from test thread only.
         */
        void Wait(f32 milliseconds) RPP_E2E_BINDING;

        void Log(const String &message) RPP_E2E_BINDING;

    private:
        /**
         * The function executed by the test thread.
         * @param arg Pointer to any arguments passed to the thread (not used here).
         */
        void TestThreadFunction(void *pParam);

        static void TestThreadStaticFunction(void *pParam);

    private:
        String m_resultFilePath;        ///< The path to the result file.
        String m_setupScriptContent;    ///< The content of the setup script.
        String m_updateScriptContent;   ///< The content of the update script.
        String m_shutdownScriptContent; ///< The content of the shutdown script.

        SignalId m_testThreadSignal; ///< Signal to control the test thread.
        SignalId m_mainThreadSignal; ///< Signal to control the main thread.
        ThreadId m_testThreadId;     ///< The ID of the test thread.

        b8 m_shouldApplicationClose; ///< Flag indicating if the application should close.

        HighResTimer m_timer; ///< Used for testing time-related functionalities.

        CommandStack m_commandStack; ///< Stack to keep track of commands executed in the test system.
                                     ///< TODO: use stack later
    };
} // namespace rpp
