#include "test_common.h"

TEST(SingletonManagerTest, EmptySingltonManager)
{
    SingletonManager::Initialize();
    SingletonManager::Shutdown();
}

TEST(SingletonManagerTest, RegisterSingletonObject)
{
}