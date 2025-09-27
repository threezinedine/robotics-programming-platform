#include "test_common.h"

TEST(JsonTest, EmtpyJson)
{
    String jsonString = R"({})";
    Json json(jsonString);

    EXPECT_TRUE(json.Empty());
}

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

TEST(JsonTest, ParserNumberField)
{
    String jsonString = R"({"name": "John", "age": 30})";
    Json json(jsonString);

    EXPECT_STREQ(json.Get<String>("name").CStr(), "John");
}

TEST(JsonTest, ParseMissingFieldWithDefault)
{
    String jsonString = R"({"name": "John", "age": 30})";
    Json json(jsonString);

    EXPECT_EQ(json.Get<i32>("height", 180), 180);
}

TEST(JsonTest, ParseExistingNumberField)
{
    String jsonString = R"({"name": "John", "age": 30})";
    Json json(jsonString);

    EXPECT_EQ(json.Get<i32>("age", 25), 30);
}

TEST(JsonTest, ParseExistingWithWrongType)
{
    String jsonString = R"({"name": "John", "age": 30})";
    Json json(jsonString);

    EXPECT_STREQ(json.Get<String>("age", "unknown").CStr(), "unknown");
}

TEST(JsonTest, ParseNumberFieldAsValue)
{
    String jsonString = R"({"name": "John", "age": 30})";
    Json json(jsonString);

    EXPECT_EQ(json.Get<u32>("age"), 30);
    EXPECT_EQ(json.Get<u16>("age"), 30);
    EXPECT_EQ(json.Get<u8>("age"), 30);
}

TEST(JsonTest, JsonTest_ParseNegativeNumber)
{
    String jsonString = R"({"temperature": -5})";
    Json json(jsonString);

    EXPECT_EQ(json.Get<i32>("temperature"), -5);
    EXPECT_EQ(json.Get<i16>("temperature"), -5);
    EXPECT_EQ(json.Get<i8>("temperature"), -5);
}

TEST(JsonTest, ParseExceedingNumberAsSmallerType)
{
    String jsonString = R"({"age": 300})";
    Json json(jsonString);

    EXPECT_FALSE(json.Get<u8>("age", 25) == 25); // 300 exceeds u8 range, should return default value
}

TEST(JsonTest, ModifiedJsonToString)
{
    String jsonString = R"({"name": "Tom", "age": 30})";
    Json json(jsonString);
    json.Set("name", String("John"));

    EXPECT_STREQ(json.ToString().CStr(),
                 R"({
    "age": 30,
    "name": "John"
})");
}

TEST(JsonTest, AddNewField)
{
    String jsonString = R"({"name": "Tom", "age": 30})";
    Json json(jsonString);
    json.Set("height", 180);

    EXPECT_STREQ(json.ToString().CStr(),
                 R"({
    "age": 30,
    "height": 180,
    "name": "Tom"
})");
}

TEST(JsonTest, BooleanField)
{
    String jsonString = R"({"isStudent": true, "isEmployed": false})";
    Json json(jsonString);

    EXPECT_STREQ(json.ToString().CStr(),
                 R"({
    "isEmployed": false,
    "isStudent": true
})");
    EXPECT_TRUE(json.Get<b8>("isStudent"));
    EXPECT_FALSE(json.Get<b8>("isEmployed"));
}

TEST(JsonTest, FloatField)
{
    String jsonString = R"({"pi": 3.14, "count": 10})";
    Json json(jsonString);

    // Float type is not supported, should return default value
    EXPECT_FLOAT_EQ(json.Get<f32>("pi", 0.0f), 3.14f);
    EXPECT_FLOAT_EQ(json.Get<f32>("nonexistent", 1.0f), 1.0f);

    EXPECT_DOUBLE_EQ(json.Get<f64>("pi", 0.0), 3.14);

    EXPECT_FLOAT_EQ(json.Get<f32>("count", 1.0f), 10.0f);
    EXPECT_EQ(json.Get<u32>("pi"), 3);
}

TEST(JsonTest, NestedJsonObject)
{
    String jsonString = R"({"person": {"name": "John", "age": 30}})";
    Json json(jsonString);
    Json child = json.Get<Json>("person");

    // Nested JSON objects are not supported, should return default value
    EXPECT_STREQ(child.Get<String>("name", "unknown").CStr(), "John");
    EXPECT_EQ(child.Get<i32>("age", 0), 30);
}

TEST(JsonTest, ObjectIsAnyArray)
{
    String jsonString = R"([1, 2, 3])";
    Json json(jsonString);

    EXPECT_TRUE(json.IsArray());
}

TEST(JsonTest, GetObjectByIndexRatherThanKey)
{
    String jsonString = R"([1, 2, 3])";
    Json json(jsonString);

    EXPECT_EQ(json.Get<i32>(0, -1), 1);
    EXPECT_EQ(json.Get<i32>(1, -1), 2);
    EXPECT_EQ(json.Get<i32>(2, -1), 3);
    EXPECT_EQ(json.Get<i32>(3, -1), -1);
}

TEST(JsonTest, ModifiedArrayElement)
{
    String jsonString = R"([1, 2, 3])";
    Json json(jsonString);
    json.Set(0, 10);

    EXPECT_STREQ(json.ToString().CStr(),
                 Json(R"([10, 2, 3])").ToString().CStr());
}

TEST(JsonTest, ArrayObjectGetSize)
{
    EXPECT_EQ(Json(R"([])").Size(), u32(0));
    EXPECT_EQ(Json(R"([1, 2, 3])").Size(), u32(3));
    EXPECT_EQ(Json(R"({"key": "value"})").Size(), u32(0));
}

TEST(JsonTest, GetJsonObjectFromKey)
{
    String jsonString = R"({"person": ["Hung", "My"]})";
    Json json(jsonString);
    Json child = json.Get<Json>("person");

    EXPECT_TRUE(child.IsArray());
    EXPECT_EQ(child.Size(), u32(2));
}