#pragma once
#if defined(RPP_USE_TEST)
#include "modules/modules.h"

namespace rpp
{
    void RPP_E2E_BINDING TestLog(const String &message);
} // namespace rpp
#endif