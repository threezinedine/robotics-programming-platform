#include "applications/test_system/utils.h"

namespace rpp
{
    void Print(const String &message)
    {
        print(message.CStr());
        print("\n");
    }
} // namespace rpp
