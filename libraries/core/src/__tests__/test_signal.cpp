#include "test_common.h"

static Array<i32> sortedElements = Array<i32>(); // Store sorted elements for verification

class SignalTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Thread::Initialize();
        Signal::Initialize();
        sortedElements.Clear();
    }

    void TearDown() override
    {
        Signal::Shutdown();
        Thread::Shutdown();
    }
};

struct SignalTestParam
{
    SignalId mainSignal;
    SignalId threadSignal;
};

void SignalTestThread(void *pParam)
{
    SignalTestParam *pTestParam = static_cast<SignalTestParam *>(pParam);
    SignalId signal = pTestParam->threadSignal;
    SignalId mainSignal = pTestParam->mainSignal;

    sortedElements.Push(3);

    Signal::Active(mainSignal); // Notify main thread
    Signal::Wait(signal);       // Wait for the signal to be activated

    sortedElements.Push(5);

    Signal::Active(mainSignal); // Notify main thread
}

TEST_F(SignalTest, CoroutineUsage)
{
    SignalId signal = Signal::Create();
    SignalId mainSignal = Signal::Create();

    SignalTestParam testParam = {mainSignal, signal};
    ThreadId threadId = Thread::Create(SignalTestThread, &testParam, sizeof(SignalTestParam));

    Thread::Start(threadId); // Start the thread

    Signal::Wait(mainSignal); // Wait for the thread to notify
    sortedElements.Push(4);
    Signal::Active(signal); // Activate the signal to let the thread proceed

    Signal::Wait(mainSignal); // Wait for the thread to notify

    // assert the sorted order
    ASSERT_EQ(sortedElements.Size(), 3);
    EXPECT_EQ(sortedElements[0], 3);
    EXPECT_EQ(sortedElements[1], 4);
    EXPECT_EQ(sortedElements[2], 5);
}