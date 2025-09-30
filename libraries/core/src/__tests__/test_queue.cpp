#include "test_common.h"
#include "core/containers/queue.h"

namespace
{
    /**
     * Testing purpose object for Queue tests.
     */
    class TestObject
    {
    public:
        static u32 instanceCount;

        TestObject()
            : TestObject(0)
        {
        }

        TestObject(i32 val)
            : value(val)
        {
            instanceCount++;
        }

        TestObject(const TestObject &other)
            : value(other.value)
        {
            instanceCount++;
        }

        TestObject(TestObject &&other)
            : value(std::move(other.value)) // Move constructor does not change the value
        {
            instanceCount++;
        }

        ~TestObject()
        {
            instanceCount--;
        }

        void operator=(const TestObject &other)
        {
            value = other.value;
        }

        i32 value = 0;
    };

    u32 TestObject::instanceCount = 0;
} // namespace

class QueueTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        TestObject::instanceCount = 0;
    }

    void TearDown() override
    {
        EXPECT_EQ(TestObject::instanceCount, 0) << "Memory leak detected in TestObject instances.";
    }
};

TEST_F(QueueTest, DefaultConstructor)
{
    Queue<u8> queue;
    EXPECT_TRUE(queue.Empty());
}

TEST_F(QueueTest, PushAndSize)
{
    Queue<u8> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    EXPECT_EQ(queue.Size(), 3);
    EXPECT_FALSE(queue.Empty());
}

TEST_F(QueueTest, PushAndAccess)
{
    Queue<u8> queue;
    queue.Push(1);
    queue.Push(2);

    EXPECT_EQ(queue.Front(), 1);
}

TEST_F(QueueTest, Pop)
{
    Queue<u8> queue;
    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    EXPECT_EQ(queue.Front(), 1);
    queue.Pop();
    EXPECT_EQ(queue.Front(), 2);
    queue.Pop();
    EXPECT_EQ(queue.Front(), 3);
    queue.Pop();
    EXPECT_TRUE(queue.Empty());
}

TEST_F(QueueTest, WithObject)
{
    Queue<TestObject> queue;
    EXPECT_EQ(TestObject::instanceCount, 0);

    queue.Push(TestObject(23));
    EXPECT_EQ(queue.Front().value, 23);
    EXPECT_EQ(TestObject::instanceCount, 1);

    queue.Push(TestObject());
    EXPECT_EQ(queue.Front().value, 23);
    EXPECT_EQ(TestObject::instanceCount, 2);

    queue.Pop();
    EXPECT_EQ(queue.Front().value, 0);
    EXPECT_EQ(TestObject::instanceCount, 1);

    queue.Pop();
    EXPECT_EQ(TestObject::instanceCount, 0);
}

TEST_F(QueueTest, SmartPointer)
{
    Queue<Scope<TestObject>> queue;
    EXPECT_EQ(TestObject::instanceCount, 0);

    queue.Push(Scope<TestObject>(new TestObject(42)));
    EXPECT_EQ(TestObject::instanceCount, 1);
}