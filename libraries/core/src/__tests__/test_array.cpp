#include "test_common.h"

/**
 * Used only for testing purposes.
 */
static struct TestStruct
{
    i32 a;
    i32 b;
};

TEST(ArrayTest, DefaultConstructor)
{
    Array<i32> arr;
    EXPECT_EQ(arr.Size(), 0);
    EXPECT_EQ(arr.Capacity(), RPP_ARRAY_DEFAULT_CAPACITY);
}

TEST(ArrayTest, ConstructorWithCapacity)
{
    Array<i32> arr(20);
    EXPECT_EQ(arr.Size(), 0);
    EXPECT_EQ(arr.Capacity(), 20);
}

TEST(ArrayTest, PushBackAndAccess)
{
    Array<i32> arr;
    arr.Push(10);
    arr.Push(20);
    arr.Push(30);
    EXPECT_EQ(arr.Size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, ModifiedElements)
{
    Array<i32> arr;
    arr.Push(10);
    arr.Push(20);
    arr.Push(30);
    arr[1] = 25;
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 25);
    EXPECT_EQ(arr[2], 30);
}

TEST(ArrayTest, PushWithInvalidIndex)
{
    Array<i8> arr;

    arr.Push(4);
    EXPECT_EQ(arr.Size(), 1);
    EXPECT_THROW(arr.Push(3, 2), std::runtime_error);
    EXPECT_THROW(arr.Push(3, -2), std::runtime_error);
    EXPECT_NO_THROW(arr.Push(3, -1));
    EXPECT_NO_THROW(arr.Push(3, 1));
}

TEST(ArrayTest, StructElement)
{
    Array<TestStruct> arr;
    TestStruct ts1 = {1, 2};
    TestStruct ts2 = {3, 4};
    arr.Push(ts1);
    arr.Push(ts2);
    EXPECT_EQ(arr.Size(), 2);
    EXPECT_EQ(arr[0].a, 1);
    EXPECT_EQ(arr[0].b, 2);
    EXPECT_EQ(arr[1].a, 3);
    EXPECT_EQ(arr[1].b, 4);
}

TEST(ArrayTest, ScopePointerElement)
{
    Array<Scope<TestStruct>> arr;
    auto ts1 = CreateScope<TestStruct>();
    ts1->a = 1;
    ts1->b = 2;
    auto ts2 = CreateScope<TestStruct>();
    ts2->a = 3;
    ts2->b = 4;
    arr.Push(ts1);
    arr.Push(ts2);
    EXPECT_EQ(arr.Size(), 2);
    EXPECT_EQ(arr[0]->a, 1);
    EXPECT_EQ(arr[0]->b, 2);
    EXPECT_EQ(arr[1]->a, 3);
    EXPECT_EQ(arr[1]->b, 4);
}

TEST(ArrayTest, RefPointerElement)
{
    Array<Ref<TestStruct>> arr;
    Ref<TestStruct> ts1 = CreateRef<TestStruct>();
    ts1->a = 1;
    ts1->b = 2;
    Ref<TestStruct> ts2 = CreateRef<TestStruct>();
    ts2->a = 3;
    ts2->b = 4;
    arr.Push(ts1);
    arr.Push(ts2);
    EXPECT_EQ(arr.Size(), 2);
    EXPECT_EQ(arr[0]->a, 1);
    EXPECT_EQ(arr[0]->b, 2);
    EXPECT_EQ(arr[1]->a, 3);
    EXPECT_EQ(arr[1]->b, 4);
}

TEST(ArrayTest, PushNonNegative1)
{
    Array<i32> arr(2);
    arr.Push(1);
    arr.Push(2, 0);

	EXPECT_EQ(arr[0], 2);
	EXPECT_EQ(arr[1], 1);
}