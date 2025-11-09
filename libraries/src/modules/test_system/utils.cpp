#if defined(RPP_USE_TEST)
#include "modules/test_system/utils.h"

namespace rpp
{
    void TestLog(const String &message)
    {
        RPP_PROFILE_SCOPE();
        print(message.CStr());
        print("\n");
    }
} // namespace rpp
#endif