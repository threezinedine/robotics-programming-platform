#include <test_common.h>

using namespace rpp;

TEST(StringTest, DefaultConstructor)
{
    String str;
    EXPECT_EQ(str.Length(), 0);
    EXPECT_STREQ(str.CStr(), "");
}