#include "test_common.h"
#include "core/containers/set.h"

namespace
{
    class TestObject
    {
    public:
        static u32 instanceCount;

    public:
        TestObject() : TestObject(0) {}
        TestObject(int v) : value(v) { instanceCount++; }
        TestObject(const TestObject &other) : value(other.value) { instanceCount++; }
        ~TestObject() { instanceCount--; }
        int value;
    };

    u32 TestObject::instanceCount = 0;

    static i8 TestObjectComparator(const void *a, const void *b)
    {
        const TestObject *objA = static_cast<const TestObject *>(a);
        const TestObject *objB = static_cast<const TestObject *>(b);

        if (objA->value == objB->value)
            return 0;
        else if (objA->value > objB->value)
            return 1;
        else
            return -1;
    }
} // namespace

class SetTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        TestObject::instanceCount = 0;
    }

    void TearDown() override
    {
        EXPECT_EQ(TestObject::instanceCount, 0);
    }
};

TEST_F(SetTest, BasicAssertions)
{
    Set<u32, defaultComparator<u32>> mySet;

    EXPECT_EQ(mySet.Size(), 0);
}

TEST_F(SetTest, AddElements)
{
    Set<u32, defaultComparator<u32>> mySet;

    EXPECT_EQ(mySet.Size(), 0);

    mySet.Add(10);
    EXPECT_EQ(mySet.Size(), 1);
}

TEST_F(SetTest, AddDuplicateElements)
{
    Set<u32, defaultComparator<u32>> mySet;

    EXPECT_EQ(mySet.Size(), 0);

    mySet.Add(10);
    EXPECT_EQ(mySet.Size(), 1);

    mySet.Add(10);              // Adding duplicate
    EXPECT_EQ(mySet.Size(), 1); // Size should remain the same
}

TEST_F(SetTest, AddMultipleUniqueElements)
{
    Set<u32, defaultComparator<u32>> mySet;

    EXPECT_EQ(mySet.Size(), 0);

    mySet.Add(20);
    EXPECT_EQ(mySet.Size(), 1);

    mySet.Add(10);
    EXPECT_EQ(mySet.Size(), 2);

    mySet.Add(30);
    EXPECT_EQ(mySet.Size(), 3);
}

TEST_F(SetTest, AccessElement)
{
    Set<u32, defaultComparator<u32>> mySet;

    EXPECT_EQ(mySet.Size(), 0);

    mySet.Add(10);
    mySet.Add(30);
    mySet.Add(20);
    EXPECT_EQ(mySet.Size(), 3);

    EXPECT_EQ(mySet[0], 10);
    EXPECT_EQ(mySet[1], 20);
    EXPECT_EQ(mySet[2], 30);
}

TEST_F(SetTest, AccessOutOfBounds)
{
    Set<u32, defaultComparator<u32>> mySet;

    EXPECT_EQ(mySet.Size(), 0);

    mySet.Add(10);
    EXPECT_EQ(mySet.Size(), 1);

    EXPECT_THROW(mySet[1], std::out_of_range); // Accessing out of bounds
}

TEST_F(SetTest, RemoveElement)
{
    Set<u32, defaultComparator<u32>> mySet;

    EXPECT_EQ(mySet.Size(), 0);

    mySet.Add(10);
    mySet.Add(30);
    mySet.Add(20);
    EXPECT_EQ(mySet.Size(), 3);

    mySet.Remove(1);
    EXPECT_EQ(mySet.Size(), 2);

    EXPECT_EQ(mySet[0], 10);
    EXPECT_EQ(mySet[1], 30);

    mySet.Remove(0);
    EXPECT_EQ(mySet.Size(), 1);
    EXPECT_EQ(mySet[0], 30);

    mySet.Add(40);
    mySet.Add(50);
    mySet.Add(10);

    mySet.Remove();
    EXPECT_EQ(mySet.Size(), 3);
    EXPECT_EQ(mySet[0], 10);
    EXPECT_EQ(mySet[1], 30);
    EXPECT_EQ(mySet[2], 40);
}

TEST_F(SetTest, Object)
{
    Set<TestObject, TestObjectComparator> mySet;

    EXPECT_EQ(mySet.Size(), 0);

    mySet.Add(TestObject(10));
    mySet.Add(TestObject(30));
    mySet.Add(TestObject(20));
    EXPECT_EQ(mySet.Size(), 3);

    EXPECT_EQ(mySet[0].value, 10);
    EXPECT_EQ(mySet[1].value, 20);
    EXPECT_EQ(mySet[2].value, 30);
}

TEST_F(SetTest, TestClear)
{
    {
        Set<TestObject, TestObjectComparator> mySet;

        EXPECT_EQ(mySet.Size(), 0);
        EXPECT_EQ(TestObject::instanceCount, 0);

        mySet.Add(TestObject(10));
        mySet.Add(TestObject(30));
        mySet.Add(TestObject(20));
        EXPECT_EQ(mySet.Size(), 3);
        EXPECT_EQ(TestObject::instanceCount, 3);

        mySet.Clear();
        EXPECT_EQ(mySet.Size(), 0);
        EXPECT_EQ(TestObject::instanceCount, 0);
    }

    // Ensure all instances are destroyed after set goes out of scope
    EXPECT_EQ(TestObject::instanceCount, 0);
}