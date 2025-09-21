#include "core/timer.h"

namespace rpp
{
    HighResTimer::HighResTimer()
        : m_highResolutionStartTimeStamp(0)
    {
    }

    HighResTimer::~HighResTimer()
    {
    }

    void HighResTimer::Start()
    {
        m_highResolutionStartTimeStamp = GetCurrentHighResolutionTimeStamp();
    }
} // namespace rpp
