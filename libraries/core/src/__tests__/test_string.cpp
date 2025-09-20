#include <test_common.h>

using namespace rpp;

TEST(StringTest, DefaultConstructor)
{
    String str;
    EXPECT_EQ(str.Length(), 0);
    EXPECT_STREQ(str.CStr(), "");
}

TEST(StringTest, ConstructorFromCStr)
{
    String str("Hello, World!");
    EXPECT_EQ(str.Length(), 13);
    EXPECT_STREQ(str.CStr(), "Hello, World!");
}

TEST(StringTest, CopyConstructor)
{
    String str1("Test String");
    String str2(str1);
    EXPECT_EQ(str2.Length(), str1.Length());
    EXPECT_STREQ(str2.CStr(), str1.CStr());
}

TEST(StringTest, AssignmentOperator)
{
    String str1("Initial String");
    String str2;
    str2 = str1;
    EXPECT_EQ(str2.Length(), str1.Length());
    EXPECT_STREQ(str2.CStr(), str1.CStr());
}

TEST(StringTest, AddString)
{
    String str;
    str += String("Hello");
    String str2 = str + String(", World!");
    EXPECT_EQ(str.Length(), 5);
    EXPECT_STREQ(str.CStr(), "Hello");
    EXPECT_EQ(str2.Length(), 13);
    EXPECT_STREQ(str2.CStr(), "Hello, World!");
}

TEST(StringTest, AccessOperator)
{
    String str("Access Test");
    EXPECT_EQ(str[0], 'A');
    EXPECT_EQ(str[7], 'T');
    EXPECT_THROW(str[20], std::out_of_range);
}

TEST(StringTest, FindSubstring)
{
    String str("Find the substring in this string. Find it well.");
    EXPECT_EQ(str.Find("Find"), 0);
    EXPECT_EQ(str.Find("substring"), 9);
    EXPECT_EQ(str.Find("notfound"), -1);
    EXPECT_EQ(str.Find("Find", 1), 35);
    EXPECT_EQ(str.Find("Find", 40), -1);
}

TEST(StringTest, ConcatStrings)
{
    String str1("Hello, ");
    String str2("World!");
    String str3 = str1.Concat(str2);
    EXPECT_EQ(str3.Length(), str1.Length() + str2.Length());
    EXPECT_STREQ(str3.CStr(), "Hello, World!");
}