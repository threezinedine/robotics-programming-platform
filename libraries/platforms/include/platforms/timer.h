#pragma once
#include "common.h"

namespace rpp
{
    /**
     * @brief Structure to hold date and time information.
     */
    struct Datetime
    {
        u16 year;        ///< Current year (e.g., 2023)
        u8 month;        ///< Month [1-12]
        u8 day;          ///< Day of the month [1-31]
        u8 hour;         ///< Hour [0-23]
        u8 minute;       ///< Minute [0-59]
        u8 second;       ///< Second [0-59]
        u16 millisecond; ///< Millisecond [0-999]
    };

    /**
     * @brief Get current UNIX timestamp.
     */
    u64 GetCurrentUNIXTimeStamp();

    /**
     * @brief Convert UNIX timestamp to Datetime structure.
     * @param timestamp UNIX timestamp to convert.
     *
     * @return Datetime structure representing the given timestamp.
     */
    Datetime TimestampToDatetime(u64 timestamp);

    /**
     * @brief Sleep for the specified number of seconds.
     * @param seconds Number of seconds to sleep.
     */
    void SleepInSeconds(u32 seconds);

    /**
     * @brief Sleep for the specified number of milliseconds.
     * @param milliseconds Number of milliseconds to sleep.
     */
    void SleepInMilliseconds(u32 milliseconds);

    /**
     * @brief Sleep for the specified number of microseconds.
     * @param microseconds Number of microseconds to sleep.
     */
    void SleepInMicroseconds(u32 microseconds);

    /**
     * @brief Get time delta upto nano seconds.
     *
     * @return Current high resolution timestamp in nanoseconds.
     */
    u64 GetCurrentHighResolutionTimeStamp();
} // namespace rpp
