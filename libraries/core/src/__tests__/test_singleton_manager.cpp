#include "test_common.h"

namespace
{
    u32 numberOfTestManagerInstances = 0;

    class TestManager
    {
        SINGLETON_DEFINE(TestManager);

    private:
        TestManager()
        {
            ++numberOfTestManagerInstances;
        }

        ~TestManager()
        {
            --numberOfTestManagerInstances;
        }

    public:
        inline u32 GetValue() const { return m_value; }
        inline void SetValue(u32 value) { m_value = value; }

    private:
        u32 m_value = 42;
    };

    //SINGLETON_IMPLEMENT(TestManager);
    TestManager* TestManager::s_instsance = nullptr; 
    TestManager* TestManager::GetInstance() {
        if (s_instsance == nullptr) {
            s_instsance = new TestManager(); 
            SingletonManager::RegisterSingleton(
                "TestManager", 
                s_instsance, 
                [](const SingletonEntry& entry) { delete static_cast<TestManager*>(entry.instance); });
        } return s_instsance;
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