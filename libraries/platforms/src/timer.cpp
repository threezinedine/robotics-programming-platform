#include "platforms/timer.h"
#include <chrono>
#include <thread>

namespace rpp
{
    u64 GetCurrentUNIXTimeStamp()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }

    Datetime TimestampToDatetime(u64 timestamp)
    {
        time_t seconds = static_cast<time_t>(timestamp / 1000);
        struct tm *timeinfo = localtime(&seconds);

        Datetime dt;
        dt.year = static_cast<u16>(timeinfo->tm_year + 1900);
        dt.month = static_cast<u8>(timeinfo->tm_mon + 1);
        dt.day = static_cast<u8>(timeinfo->tm_mday);
        dt.hour = static_cast<u8>(timeinfo->tm_hour);
        dt.minute = static_cast<u8>(timeinfo->tm_min);
        dt.second = static_cast<u8>(timeinfo->tm_sec);
        dt.millisecond = static_cast<u16>(timestamp % 1000);

        return dt;
    }

    u64 ConvertDeltaTimeToSeconds(u64 deltaTime)
    {
        return deltaTime / 1000;
    }

    void SleepInSeconds(u32 seconds)
    {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }

    void SleepInMilliseconds(u32 milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    void SleepInMicroseconds(u32 microseconds)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
    }

    u64 GetCurrentHighResolutionTimeStamp()
    {
        using namespace std::chrono;
        return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
    }
} // namespace rpp
