#include "test_common.h"

class TestSystem : public System
{
public:
    static u32 numberOfObjects;
    static u32 initialCallCount;
    static u32 updateCallCount;
    static u32 shutdownCallCount;

    TestSystem() { numberOfObjects++; }
    ~TestSystem() override { numberOfObjects--; }

protected:
    void InitialImpl(ECSId id) override { initialCallCount++; }
    void UpdateImpl(ECSId id, f32 deltaTime) override { updateCallCount++; }
    void ShutdownImpl(ECSId id) override { shutdownCallCount++; }
};

u32 TestSystem::numberOfObjects = 0;
u32 TestSystem::initialCallCount = 0;
u32 TestSystem::updateCallCount = 0;
u32 TestSystem::shutdownCallCount = 0;

#define COMPONENT_A_ID 1
#define COMPONENT_B_ID 2

struct ComponentA
{
    int a;
};

struct ComponentB
{
    float b;
};
class ECSAssert
{
public:
    static void AssertNumberOfEntities(u32 expectedNumberOfEntities)
    {
        auto ecsData = ECS::s_ecsStorage->GetNumberOfElements();
        ASSERT_EQ(ecsData, expectedNumberOfEntities);
        ASSERT_EQ(TestSystem::numberOfObjects, 0);
    }

    static ECS::ECSData::SystemData *GetSystemData(ECSId ecsId, SystemId systemId)
    {
        RPP_ASSERT(ECS::s_ecsStorage != nullptr);
        RPP_ASSERT(ecsId != INVALID_ID);

        ECS::ECSData *currentECS = ECS::s_ecsStorage->Get(ecsId);
        RPP_ASSERT(currentECS != nullptr);

        return currentECS->systemStorage->Get(systemId);
    }

    static b8 IsEntityMatchSystem(Entity *pEntity, ECS::ECSData::SystemData *pSystemData)
    {
        return ECS::IsEntityMatchSystem(pEntity, pSystemData);
    }
};

class ECSTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        SingletonManager::Initialize();
        rpp::ECS::Initialize();
        TestSystem::initialCallCount = 0;
        TestSystem::updateCallCount = 0;
        TestSystem::shutdownCallCount = 0;
    }

    void TearDown() override
    {
        rpp::ECS::Shutdown();
        SingletonManager::Shutdown();
    }
};

#define SINGLE_ECS_SETUP()    \
    ECSId id = ECS::Create(); \
    ECS::Activate(id);

#define SYSTEM_SETUP(system, ...)                     \
    ComponentId requiredComponents[] = {__VA_ARGS__}; \
    u32 systemId = ECS::RegisterSystem(RPP_NEW(system()), requiredComponents, sizeof(requiredComponents) / sizeof(ComponentId));

#define CREATE_ENTITY_WITH_AB_COMPONENTS()                                \
    ComponentA aData = {42};                                              \
    Component aComponent = {COMPONENT_A_ID, TRUE, &aData, sizeof(aData)}; \
    ComponentB bData = {3.14f};                                           \
    Component bComponent = {COMPONENT_B_ID, TRUE, &bData, sizeof(bData)}; \
    Component *components[] = {&aComponent, &bComponent};                 \
    EntityId entityId = ECS::CreateEntity(components, 2);                 \
    Entity *entity = ECS::GetEntity(entityId);

#define CREATE_ENTITY_WITH_A_COMPONENT()                                  \
    ComponentA aData = {42};                                              \
    Component aComponent = {COMPONENT_A_ID, TRUE, &aData, sizeof(aData)}; \
    Component *components[] = {&aComponent};                              \
    EntityId entityId = ECS::CreateEntity(components, 1);                 \
    Entity *entity = ECS::GetEntity(entityId);

#define CREATE_ENTITY_WITH_B_COMPONENT()                                  \
    ComponentB bData = {3.14f};                                           \
    Component bComponent = {COMPONENT_B_ID, TRUE, &bData, sizeof(bData)}; \
    Component *components[] = {&bComponent};                              \
    EntityId entityId = ECS::CreateEntity(components, 1);                 \
    Entity *entity = ECS::GetEntity(entityId);

TEST_F(ECSTest, CreateSimpleECSInstance)
{
    ECSId id = ECS::Create();
    ECSAssert::AssertNumberOfEntities(1);
}

TEST_F(ECSTest, CheckAnEntityMatchASystem)
{
    SINGLE_ECS_SETUP();

    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_AB_COMPONENTS();

    auto pSystemData = ECSAssert::GetSystemData(id, 0);
    ASSERT_TRUE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));
}

TEST_F(ECSTest, CheckNotMatch)
{
    SINGLE_ECS_SETUP();

    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID, COMPONENT_B_ID);
    CREATE_ENTITY_WITH_A_COMPONENT();

    auto pSystemData = ECSAssert::GetSystemData(id, 0);
    ASSERT_FALSE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));
}

TEST_F(ECSTest, CheckNotMatchIfInactive)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);

    ComponentA aData = {42};
    Component aComponent = {1, FALSE, &aData, sizeof(aData)};
    Component *components[] = {&aComponent};
    EntityId entityId = ECS::CreateEntity(components, 1);
    Entity *entity = ECS::GetEntity(entityId);

    auto pSystemData = ECSAssert::GetSystemData(id, 0);
    ASSERT_FALSE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));
}

TEST_F(ECSTest, ModifyTheStatusOfTheComponent)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);

    auto pSystemData = ECSAssert::GetSystemData(id, 0);

    ComponentA aData = {42};
    Component aComponent = {COMPONENT_A_ID, FALSE, &aData, sizeof(aData)};
    Component *components[] = {&aComponent};
    EntityId entityId = ECS::CreateEntity(components, 1);
    Entity *entity = ECS::GetEntity(entityId);

    ASSERT_FALSE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));

    ECS::ModifyComponentStatus(entityId, COMPONENT_A_ID, TRUE);
    ASSERT_FALSE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));

    ECS::Update(0.016f);

    ASSERT_TRUE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));
}

TEST_F(ECSTest, DeleteTheEntity)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_A_COMPONENT();

    ECS::DestroyEntity(entityId);
    ASSERT_TRUE(ECS::GetEntity(entityId) != nullptr);

    ECS::Update(0.016f);

    ASSERT_EQ(ECS::GetEntity(entityId), nullptr);
}

TEST_F(ECSTest, ModifyStatusOfComponentWhichItsEntityIsDeleted)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_A_COMPONENT();

    ECS::DestroyEntity(entityId);
    ASSERT_TRUE(ECS::GetEntity(entityId) != nullptr);

    ECS::ModifyComponentStatus(entityId, COMPONENT_A_ID, FALSE);

    ASSERT_NO_THROW(ECS::Update(0.016f));
    ASSERT_EQ(ECS::GetEntity(entityId), nullptr);
}

TEST_F(ECSTest, ModifyTheStatusOfTheEntity)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_A_COMPONENT();

    auto pSystemData = ECSAssert::GetSystemData(id, 0);

    ASSERT_TRUE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));

    ECS::ModifyEntityStatus(entityId, FALSE);
    ASSERT_TRUE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));

    ECS::Update(0.016f);

    ASSERT_FALSE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));

    ECS::ModifyEntityStatus(entityId, TRUE);
    ASSERT_FALSE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));

    ECS::Update(0.016f);

    ASSERT_TRUE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));
}

TEST_F(ECSTest, ModifyStatusOfTheSystem)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);

    auto pSystemData = ECSAssert::GetSystemData(id, systemId);
    ASSERT_TRUE(pSystemData->isActive);

    ECS::ModifySystemStatus(systemId, FALSE);
    pSystemData = ECSAssert::GetSystemData(id, systemId);
    ASSERT_TRUE(pSystemData->isActive);

    ECS::Update(0.016f);

    ASSERT_FALSE(pSystemData->isActive);
}

TEST_F(ECSTest, DISABLED_CreateEntityMatchSystem)
{
    ECSId id = ECS::Create();

    ECS::Activate(id);

    ComponentId requiredComponents[] = {COMPONENT_A_ID};
    ECS::RegisterSystem(RPP_NEW(TestSystem()), requiredComponents, 1);
    ASSERT_EQ(TestSystem::initialCallCount, 0);
    ASSERT_EQ(TestSystem::numberOfObjects, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);
    ASSERT_EQ(TestSystem::numberOfObjects, 1);

    ComponentA aData = {42};
    Component aComponent = {COMPONENT_A_ID, TRUE, &aData, sizeof(aData)};
    ComponentB bData = {3.14f};
    Component bComponent = {COMPONENT_B_ID, TRUE, &bData, sizeof(bData)};
    Component *components[] = {&aComponent, &bComponent};

    EntityId entityId = ECS::CreateEntity(components, 2);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECSAssert::AssertNumberOfEntities(1);

    ECS::Update(0.016f);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 1);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(0.016f);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 2);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::DestroyEntity(entityId);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 1);
    ASSERT_EQ(TestSystem::shutdownCallCount, 1);
}