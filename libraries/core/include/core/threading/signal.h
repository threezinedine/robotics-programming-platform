#pragma once
#include "platforms/platforms.h"
#include "core/containers/storage.h"

#define INFINITE_WAIT 0

namespace rpp
{
    /**
     * Used for identifying signals.
     */
    typedef u32 SignalId;

    /**
     * The Signaling system provides the way to create the coroutine concept inside RPP project
     */
    class Signal
    {
    private:
        /**
         * Hold context information for a signal.
         * @note Currently this struct is empty, but it can be extended in the future if needed.
         */
        struct SignalData
        {
            void *pData; ///< Pointer to user-defined data associated with the signal.
        };

    public:
        /// @brief Initializes the signal handling system.
        static void
        Initialize();

        /**
         * @brief Shuts down the signal handling system.
         */
        static void Shutdown();

    public:
        /**
         * @brief Creates a new signal and returns its identifier.
         * @return The identifier of the newly created signal.
         */
        static SignalId Create();

        /**
         * @brief Destroys the specified signal.
         * @param signal The identifier of the signal to destroy.
         * @note This function is thread-safe.
         */
        static void Destroy(SignalId signal);

        /**
         * @brief Hangs up the current thread until the specified signal is triggered or the timeout expires.
         * @param signal The identifier of the signal to wait for.
         * @param timeout The maximum time to wait in milliseconds. A value of ``INFINITE_WAIT`` means to wait indefinitely.
         * @return True if the signal was triggered, false if the timeout expired.
         */
        static b8 Wait(SignalId signal, u32 timeout = INFINITE_WAIT);

        /**
         * When a thread calls this function, it triggers the specified signal, other threads who are waiting for this signal will be resumed.
         * @param signal The identifier of the signal to trigger.
         *
         * @note the current thread will not be suspended after calling this function.
         */
        static void Active(SignalId signal);

    private:
        static Scope<Storage<SignalData>> s_signals; ///< Storage for signal context data.
    };
} // namespace rpp
