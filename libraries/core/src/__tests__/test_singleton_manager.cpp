#include "test_common.h"

namespace
{
    u32 numberOfTestManagerInstances = 0;

    class TestManager
    {
        RPP_SINGLETON_DEFINE(TestManager);

    public:
        inline u32 GetValue() const { return m_value; }
        inline void SetValue(u32 value) { m_value = value; }

    private:
        u32 m_value = 42;
    };

    RPP_SINGLETON_IMPLEMENT(TestManager);

    TestManager::TestManager()
    {
        ++numberOfTestManagerInstances;
    }

    TestManager::~TestManager()
    {
        --numberOfTestManagerInstances;
    }
} // namespace

TEST(SingletonManagerTest, EmptySingltonManager)
{
    SingletonManager::Initialize();
    SingletonManager::Shutdown();
}

TEST(SingletonManagerTest, RegisterSingletonObject)
{
    SingletonManager::Initialize();

    EXPECT_EQ(numberOfTestManagerInstances, 0);

    TestManager::GetInstance()->SetValue(100);
    EXPECT_EQ(TestManager::GetInstance()->GetValue(), 100);

    EXPECT_EQ(numberOfTestManagerInstances, 1);

    SingletonManager::Shutdown();
    EXPECT_EQ(numberOfTestManagerInstances, 0);
}