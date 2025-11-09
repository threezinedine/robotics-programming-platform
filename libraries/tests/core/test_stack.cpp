#include "test_common.h"

TEST(StackTest, BasicOperations)
{
    Stack<int> stack;

    EXPECT_TRUE(stack.Empty());
    EXPECT_EQ(stack.Size(), 0);

    stack.Push(10);
    stack.Push(20);
    stack.Push(30);

    EXPECT_FALSE(stack.Empty());
    EXPECT_EQ(stack.Size(), 3);
    EXPECT_EQ(stack.Top(), 30);

    stack.Pop();
    EXPECT_EQ(stack.Size(), 2);
    EXPECT_EQ(stack.Top(), 20);

    stack.Pop();
    stack.Pop();
    EXPECT_TRUE(stack.Empty());
} // TODO: More test for edge cases and exception handling.