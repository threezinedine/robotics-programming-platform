#include "modules/test_system/utils.h"

namespace rpp
{
    void TestLog(const String &message)
    {
        print(message.CStr());
        print("\n");
    }
} // namespace rpp
