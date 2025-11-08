#pragma once
#include "common.h"
#include "platforms/platforms.h"

namespace rpp
{
    /**
     * Used to measure high resolution time intervals.
     */
    class HighResTimer
    {
    public:
        HighResTimer();
        ~HighResTimer();

        /**
         * @brief Start the timer. Can call this method as many times as needed to reset the start time.
         */
        void Start();

        inline f64 GetElapsedTimeInSeconds() const { return GetElapsedTimeInMilliseconds() / 1000; }
        inline f64 GetElapsedTimeInMilliseconds() const { return GetElapsedTimeInMicroseconds() / 1000; }
        inline f64 GetElapsedTimeInMicroseconds() const { return GetElapsedTimeInNanoseconds() / 1000; }
        inline f64 GetElapsedTimeInNanoseconds() const { return f64(GetCurrentHighResolutionTimeStamp() - m_highResolutionStartTimeStamp); }

    private:
        u64 m_highResolutionStartTimeStamp = 0; ///< Start timestamp in nanoseconds.
    };
} // namespace rpp
