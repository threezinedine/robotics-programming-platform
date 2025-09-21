#include "test_common.h"

TEST(JsonTest, ParseStringField)
{
    String jsonString = R"({"name": "John", "age": 30})";
    Json json(jsonString);
    EXPECT_STREQ(json.ToString().CStr(),
                 R"({
    "age": 30,
    "name": "John"
})");
}