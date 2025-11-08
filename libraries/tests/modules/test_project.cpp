#include "test_common.h"

TEST(ProjectTest, ConvertToString)
{
    ProjectDescription desc;
    desc.name = "TestProject";

    EXPECT_STREQ(ToString(desc).CStr(), Json(R"({"name": "TestProject", "functionNames": []})").ToString().CStr());
}