#include "test_common.h"

class ThreadingTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Thread::Initialize();
    }

    void TearDown() override
    {
        Thread::Shutdown();
    }
};

static Array<i32> sortedElements; // Store sorted elements for verification

void SortingTestThread(void *param)
{
    i32 *pElement = static_cast<i32 *>(param);

    Thread::Sleep(*pElement * 10); // sleep for element * 10 milliseconds
    sortedElements.Push(*pElement);
}

TEST_F(ThreadingTest, ThreadSort)
{
    i32 array[] = {5, 3, 8, 1, 4};
    Array<ThreadId> threads;

    // thread sorting for testing purpose
    for (u32 i = 0; i < sizeof(array) / sizeof(i32); i++)
    {
        ThreadId threadId = Thread::Create(SortingTestThread, &array[i], sizeof(i32));
        threads.Push(threadId);
    }

    // start all threads
    for (u32 i = 0; i < threads.Size(); i++)
    {
        Thread::Start(threads[i]);
    }

    // wait for all threads to finish
    for (u32 i = 0; i < threads.Size(); i++)
    {
        Thread::Join(threads[i]);
        Thread::Destroy(threads[i]);
    }

    // Verify the sorted order
    ASSERT_EQ(sortedElements.Size(), sizeof(array) / sizeof(i32));
    for (u32 i = 1; i < sortedElements.Size(); i++)
    {
        EXPECT_LE(sortedElements[i - 1], sortedElements[i]) << "Elements are not sorted correctly.";
    }
}