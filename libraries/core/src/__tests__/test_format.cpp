#include "test_common.h"

TEST(FormatTest, BasicFormatString)
{
    String formatted = Format("Hello, {}!", String("World"));
    EXPECT_STREQ(formatted.CStr(), "Hello, World!");
}

TEST(FormatTest, FormatMultipleValues)
{
    String formatted = Format("The answer is {} and the question is '{}'.", 42, String("What is the meaning of life?"));
    EXPECT_STREQ(formatted.CStr(), "The answer is 42 and the question is 'What is the meaning of life?'.");
}