#pragma once
#include "platforms/platforms.h"
#include "core/containers/storage.h"

namespace rpp
{
    /**
     * @brief The function signature for thread entry functions.
     * @param param A pointer to the parameter data passed to the thread. Can be nullptr.
     * @note The parameter data is copied into the thread context when the thread is created, so the pointer should remain valid until the thread is destroyed.
     */
    typedef void (*ThreadEntry)(void *);

    typedef u32 ThreadId; ///< Type used for identifying threads.

    class Thread
    {
    private:
        /**
         * Hold context information for a thread. When creating the thread, the data will be passed to the entry function.
         * @note The data will be automatically copied into the storage, not related to the created thread context, so if wanna
         * use pointer inside the data, make sure the pointer is valid until the thread is destroyed.
         */
        struct ThreadData
        {
            ThreadEntry entry; ///< The function to be executed by the thread.
            void *pData;       ///< The parameter to be passed to the entry function. Can be nullptr.
            u32 dataSize;      ///< The size of the parameter data in bytes.
            b8 isRunning;      ///< Indicates whether the thread is currently running.
            void *pHandle;     ///< The handle to the underlying thread object.
        };

    public:
        /// @brief Initializes the threading system.
        static void Initialize();

        /**
         * @brief Shuts down the threading system.
         */
        static void Shutdown();

    public:
        /**
         * @brief Creates a new thread.
         * @param entry The function to be executed by the thread.
         * @param param The parameter to be passed to the entry function. Can be nullptr.
         * @param paramSize The size of the parameter data in bytes. Ignored if param is nullptr.
         * @return The identifier of the newly created thread.
         */
        static ThreadId Create(ThreadEntry entry, void *param = nullptr, u32 paramSize = 0);

        /**
         * @brief Starts the execution of the specified thread.
         * @param threadId The identifier of the thread to start.
         */
        static void Start(ThreadId threadId);

        /**
         * @brief Waits for the specified thread to finish execution.
         * @param threadId The identifier of the thread to wait for.
         */
        static void Join(ThreadId threadId);

        /**
         * @brief Suspends the execution of the current thread for the specified duration.
         * @param milliseconds The duration to sleep in milliseconds.
         *
         * @note This function affects the calling thread, not the thread identified by threadId.
         */
        static void Sleep(u32 milliseconds);

        /**
         * @brief Destroys the specified thread.
         * @param threadId The identifier of the thread to destroy.
         * @note This function is thread-safe.
         */
        static void Destroy(ThreadId threadId);

    private:
        static Scope<Storage<ThreadData>> s_threads; ///< Storage for thread context data.
    };
} // namespace rpp
