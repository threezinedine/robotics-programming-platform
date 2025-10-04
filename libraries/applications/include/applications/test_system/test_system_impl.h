#pragma once
#include "modules/modules.h"

namespace rpp
{
    /**
     * Used for e2e testing of the applications module
     */
    class TestSystem
    {
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
        static void Initialize(const String &resultFilePath,
                               const String &setupFilePath = "",
                               const String &updateFilePath = "",
                               const String &shutdownFilePath = "");

        /**
         * Shuts down the test system, releasing all resources.
         */
        static void Shutdown();

    public:
        /**
         * Updates the test system by executing the update script if provided.
         *
         * @param deltaTime The time elapsed since the last update call.
         *
         * @note this function will be called from the main thread, with locking mechanism for coroutine behavior
         */
        static void Update(f32 deltaTime);

        /**
         * @brief Checks if the test system should close, typically when all tests are done.
         * @return TRUE if the test system should close, FALSE otherwise.
         */
        static b8 ShouldApplicationClose();

    public:
        /**
         * @brief Yields the execution of the current thread, allowing other threads to run.
         * @note This function is useful for implementing coroutine-like behavior in the test system. Only
         * be called from the test thread.
         */
        static void Yield();

    private:
        static void TestThreadFunction(void *pParam);

    private:
        static String s_resultFilePath;        ///< The path to the result file.
        static String s_setupScriptContent;    ///< The content of the setup script.
        static String s_updateScriptContent;   ///< The content of the update script.
        static String s_shutdownScriptContent; ///< The content of the shutdown script.

        static SignalId s_testThreadSignal; ///< Signal to control the test thread.
        static SignalId s_mainThreadSignal; ///< Signal to control the main thread.
        static ThreadId s_testThreadId;     ///< The ID of the test thread.

        static b8 s_shouldApplicationClose; ///< Flag indicating if the application should close.
    };
} // namespace rpp
