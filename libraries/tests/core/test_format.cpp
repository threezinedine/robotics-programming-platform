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

TEST(FormatTest, BooleanFormatting)
{
    String formattedTrue = Format("Boolean value: {}", TRUE);
    String formattedFalse = Format("Boolean value: {}", FALSE);
    EXPECT_STREQ(formattedTrue.CStr(), "Boolean value: true");
    EXPECT_STREQ(formattedFalse.CStr(), "Boolean value: false");
}

TEST(FormatTest, FloatFormatting)
{
    String formatted = Format("Pi is approximately {}.", 3.14159f);
    EXPECT_STREQ(formatted.CStr(), "Pi is approximately 3.14.");
}

TEST(FormatTest, NoPlaceholders)
{
    String formatted = Format("No placeholders here.", 123);
    EXPECT_STREQ(formatted.CStr(), "No placeholders here.");
}

TEST(FormatTest, MissingArguments)
{
    String formatted = Format("Only one placeholder: {}: {}", 123);
    EXPECT_STREQ(formatted.CStr(), "Only one placeholder: 123: {}");
}