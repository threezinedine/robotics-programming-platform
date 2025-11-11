#include "test_common.h"

static b8 executed = false;
static b8 undone = false;

class CommandTestClass : public Command
{
    RPP_DEFINE_COMMAND(CommandTestClass, 0x00);

protected:
    void ExecuteImpl() override
    {
        executed = true;
    }

    void UndoImpl() override
    {
        undone = true;
    }
};

class HistoryTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        SingletonManager::Initialize();
    }

    void TearDown() override
    {
        SingletonManager::Shutdown();
    }
};

TEST_F(HistoryTest, ExecuteNullCommand)
{
    HistoryManager *pHistoryManager = HistoryManager::GetInstance();

    EXPECT_FALSE(executed);
    EXPECT_FALSE(undone);

    pHistoryManager->ExecuteCommand(nullptr);

    EXPECT_NO_THROW(pHistoryManager->Undo());

    EXPECT_FALSE(executed);
    EXPECT_FALSE(undone);

    pHistoryManager->ExecuteCommand(RPP_NEW(CommandTestClass));

    EXPECT_TRUE(executed);
    EXPECT_FALSE(undone);

    pHistoryManager->Undo();

    EXPECT_TRUE(executed);
    EXPECT_TRUE(undone);
}

// TODO: More tests for edge cases and additional functionalities.