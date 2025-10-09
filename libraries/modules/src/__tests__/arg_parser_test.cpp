#include "test_common.h"

#define DEFINE_ARGS(...)          \
    char *args[] = {__VA_ARGS__}; \
    int argc = sizeof(args) / sizeof(char *)

#define EXPECT_JSON_EQ(json1, json2) EXPECT_STREQ(json1.ToString().CStr(), json2.ToString().CStr())

TEST(ArgParserTest, EmptyParser)
{
    DEFINE_ARGS("main.exe");

    ArgParser parser;

    Json result = parser.Parse(argc, args);

    EXPECT_JSON_EQ(result, Json(R"({})"));
}

TEST(ArgParserTest, SimpleArgs)
{
    DEFINE_ARGS("main.exe", "input.txt");

    ArgParser parser;
    parser.AddArgument(ArgumentData{"scenario"});

    Json result = parser.Parse(argc, args);

    EXPECT_JSON_EQ(result, Json(R"({
        "scenario": "input.txt"
    })"));
}

TEST(ArgParserTest, ThrowErrorIfNotPassedRequiredArgs)
{
    DEFINE_ARGS("main.exe");

    ArgParser parser;
    parser.AddArgument(ArgumentData{"scenario"});

    EXPECT_THROW(parser.Parse(argc, args), std::runtime_error);
}