#include "test_common.h"
#include "core/containers/list.h"

namespace
{
    class TestObject
    {
    public:
        static i32 instanceCount;

    public:
        TestObject()
            : TestObject(0)
        {
        }

        TestObject(i32 v)
            : value(v)
        {
            instanceCount++;
            value = v;
        }

        ~TestObject()
        {
            instanceCount--;
        }

        i32 value;
    };

    i32 TestObject::instanceCount = 0;
} // namespace

TEST(ListTest, DefaultConstructor)
{
    List<i32> list;
    EXPECT_EQ(list.Size(), 0);
}

TEST(ListTest, SizeAfterInsertions)
{
    List<i32> list;
    list.Push(10);

    EXPECT_EQ(list.Size(), 1);
}

TEST(ListTest, AccessIndex)
{
    List<i32> list;
    list.Push(10);
    list.Push(20);
    list.Push(30);

    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 20);
    EXPECT_EQ(list[2], 30);

    EXPECT_EQ(list.Size(), 3);
}

TEST(ListTest, AccessOutOfRange)
{
    List<i32> list;
    list.Push(10);
    list.Push(20);

    EXPECT_THROW(list[2], std::out_of_range);
    EXPECT_THROW(list[5], std::out_of_range);
}

TEST(ListTest, InsertAtNonNull)
{
    List<i32> list;
    list.Push(10);
    list.Push(20, 0); // Index parameter is ignored in current implementation
    list.Push(30, 1); // Index parameter is ignored in current implementation

    EXPECT_EQ(list[0], 20);
    EXPECT_EQ(list[1], 30);
    EXPECT_EQ(list[2], 10);

    EXPECT_EQ(list.Size(), 3);
}

TEST(ListTest, InsertAtInvalidIndex)
{
    List<i32> list;
    list.Push(10);
    list.Push(20);

    // Current implementation ignores index parameter, so no exception is thrown
    EXPECT_THROW(list.Push(30, 5), std::runtime_error);
}

TEST(ListTest, InsertEmptyArrayWithInvalidIndex)
{
    List<i32> list;

    EXPECT_THROW(list.Push(10, 1), std::runtime_error);
    EXPECT_THROW(list.Push(20, 5), std::runtime_error);
}

TEST(ListTest, InsertObject)
{
    List<TestObject> list;

    {
        list.Push(TestObject(10));
        list.Push(TestObject(20));

        EXPECT_EQ(list.Size(), 2);
        EXPECT_EQ(list[0].value, 10);
        EXPECT_EQ(list[1].value, 20);

        EXPECT_EQ(TestObject::instanceCount, 2); // No dynamic allocations in current implementation
    }

    list.~List<TestObject>();
    EXPECT_EQ(TestObject::instanceCount, 0);
}

TEST(ListTest, ClearList)
{
    List<TestObject> list;

    list.Push(TestObject(10));
    list.Push(TestObject(20));
    list.Push(TestObject(30));

    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(TestObject::instanceCount, 3); // No dynamic allocations in current implementation

    list.Clear();
    EXPECT_EQ(list.Size(), 0);
    EXPECT_EQ(TestObject::instanceCount, 0);
}

TEST(ListTest, EraseElement)
{
    List<i32> list;

    EXPECT_THROW(list.Erase(0), std::runtime_error); // List is empty

    list.Push(10);
    list.Push(20);
    list.Push(30);

    EXPECT_EQ(list.Size(), 3);

    list.Erase(1); // Remove element at index 1 (value 20)

    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 30);

    EXPECT_THROW(list[2], std::out_of_range);
}

TEST(ListTest, EraseTestObject)
{
    List<TestObject> list;

    list.Push(TestObject(10));
    list.Push(TestObject(20));
    list.Push(TestObject(30));
    list.Push(TestObject(40));

    EXPECT_EQ(list.Size(), 4);
    EXPECT_EQ(TestObject::instanceCount, 4); // No dynamic allocations in current implementation

    list.Erase(1); // Remove element at index 1 (value 20)

    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(list[0].value, 10);
    EXPECT_EQ(list[1].value, 30);
    EXPECT_EQ(list[2].value, 40);
    EXPECT_EQ(TestObject::instanceCount, 3); // No dynamic allocations in current implementation

    list.Erase(0); // Remove element at index 0 (value 10)

    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list[0].value, 30);
    EXPECT_EQ(list[1].value, 40);
    EXPECT_EQ(TestObject::instanceCount, 2); // No dynamic allocations in current implementation

    list.Erase(); // Remove last element

    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list[0].value, 30);
    EXPECT_EQ(TestObject::instanceCount, 1);
}