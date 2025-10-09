#include "test_common.h"

#define DEFINE_ARGS(...)          \
    char *args[] = {__VA_ARGS__}; \
    int argc = sizeof(args) / sizeof(char *)

TEST(ArgParserTest, EmptyParser)
{
    DEFINE_ARGS("main.exe");

    ArgParser parser;

    Json result = parser.Parse(argc, args);

    EXPECT_EQ(result.ToString(), "{}");
}