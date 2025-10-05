#include "test_common.h"

#if defined(RPP_PLATFORM_WINDOWS)
#define ECS_TEST_DELTA_TIME 0.016f

#define COMPONENT_A_ID 1
#define COMPONENT_B_ID 2
#define COMPONENT_C_ID 3

class TestSystem : public System
{
public:
    static u32 numberOfObjects;
    static u32 initialCallCount;
    static u32 suspendCallCount;
    static u32 updateCallCount;
    static u32 resumeCallCount;
    static u32 shutdownCallCount;

    TestSystem() { numberOfObjects++; }
    ~TestSystem() override { numberOfObjects--; }

protected:
    void InitialImpl(ECSId id, EntityId entityId) override { initialCallCount++; }
    void SuspendImpl(ECSId id, EntityId entityId) override { suspendCallCount++; }
    void UpdateImpl(ECSId id, EntityId entityId, f32 deltaTime) override { updateCallCount++; }
    void ResumeImpl(ECSId id, EntityId entityId) override { resumeCallCount++; }
    void ShutdownImpl(ECSId id, EntityId entityId) override { shutdownCallCount++; }
};

u32 TestSystem::numberOfObjects = 0;
u32 TestSystem::initialCallCount = 0;
u32 TestSystem::suspendCallCount = 0;
u32 TestSystem::updateCallCount = 0;
u32 TestSystem::resumeCallCount = 0;
u32 TestSystem::shutdownCallCount = 0;

struct ComponentA
{
    int a;
};

class AddComponentASystem : public System
{
protected:
    void UpdateImpl(ECSId ecsId, EntityId entityId, f32 deltaTime) override
    {
        Component *pComponent = ECS::GetComponent(entityId, COMPONENT_A_ID);
        ComponentA *pData = (ComponentA *)pComponent->pData;
        pData->a++;
    }
};

struct ComponentB
{
    float b;
};

struct ComponentC
{
    char c;
};

class ECSAssert
{
public:
    static void AssertNumberOfEntities(u32 expectedNumberOfEntities)
    {
        auto ecsData = ECS::s_ecsStorage->GetNumberOfElements();
        ASSERT_EQ(ecsData, expectedNumberOfEntities);
    }

    static ECS::ECSData::SystemData *GetSystemData(ECSId ecsId, SystemId systemId)
    {
        RPP_ASSERT(ECS::s_ecsStorage != nullptr);
        RPP_ASSERT(ecsId != INVALID_ID);

        ECS::ECSData *pCurrentECS = ECS::s_ecsStorage->Get(ecsId);
        RPP_ASSERT(pCurrentECS != nullptr);

        return pCurrentECS->systemStorage->Get(systemId);
    }

    static b8 IsEntityMatchSystem(Entity *pEntity, ECS::ECSData::SystemData *pSystemData)
    {
        return ECS::IsEntityMatchSystem(pEntity, pSystemData);
    }

    static b8 IsEntityInSystemMatchEntities(ECSId ecsId, SystemId systemId, EntityId entityId)
    {
        RPP_ASSERT(ECS::s_ecsStorage != nullptr);
        RPP_ASSERT(ecsId != INVALID_ID);

        ECS::ECSData *currentECS = ECS::s_ecsStorage->Get(ecsId);
        RPP_ASSERT(currentECS != nullptr);

        ECS::ECSData::SystemData *pSystemData = currentECS->systemStorage->Get(systemId);

        u32 matchEntitiesCount = pSystemData->matchedEntities.Size();
        for (u32 matchEntityIdIndex = 0; matchEntityIdIndex < matchEntitiesCount; ++matchEntityIdIndex)
        {
            if (pSystemData->matchedEntities[matchEntityIdIndex] == entityId)
            {
                return TRUE;
            }
        }

        return FALSE;
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
        TestSystem::suspendCallCount = 0;
        TestSystem::updateCallCount = 0;
        TestSystem::resumeCallCount = 0;
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

#define CREATE_ENTITY_WITH_AB_COMPONENTS(valueA, valueB)                  \
    ComponentA aData = {u32(valueA)};                                     \
    Component aComponent = {COMPONENT_A_ID, TRUE, &aData, sizeof(aData)}; \
    ComponentB bData = {f32(valueB)};                                     \
    Component bComponent = {COMPONENT_B_ID, TRUE, &bData, sizeof(bData)}; \
    Component *components[] = {&aComponent, &bComponent};                 \
    EntityId entityId = ECS::CreateEntity(components, 2);                 \
    Entity *entity = ECS::GetEntity(entityId);

#define CREATE_ENTITY_WITH_A_COMPONENT(valueA)                            \
    ComponentA aData = {u32(valueA)};                                     \
    Component aComponent = {COMPONENT_A_ID, TRUE, &aData, sizeof(aData)}; \
    Component *components[] = {&aComponent};                              \
    EntityId entityId = ECS::CreateEntity(components, 1);                 \
    Entity *entity = ECS::GetEntity(entityId);

#define CREATE_ENTITY_WITH_B_COMPONENT(valueB)                            \
    ComponentB bData = {f32(valueB)};                                     \
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

    ECS::Update(ECS_TEST_DELTA_TIME);

    ASSERT_TRUE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));
}

TEST_F(ECSTest, DeleteTheEntity)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_A_COMPONENT();

    ECS::DestroyEntity(entityId);
    ASSERT_TRUE(ECS::GetEntity(entityId) != nullptr);

    ECS::Update(ECS_TEST_DELTA_TIME);

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

    ASSERT_NO_THROW(ECS::Update(ECS_TEST_DELTA_TIME));
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

    ECS::Update(ECS_TEST_DELTA_TIME);

    ASSERT_FALSE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));

    ECS::ModifyEntityStatus(entityId, TRUE);
    ASSERT_FALSE(ECSAssert::IsEntityMatchSystem(entity, pSystemData));

    ECS::Update(ECS_TEST_DELTA_TIME);

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

    ECS::Update(ECS_TEST_DELTA_TIME);

    ASSERT_FALSE(pSystemData->isActive);
}

TEST_F(ECSTest, RetrieveTheComponentFromEntity)
{
    SINGLE_ECS_SETUP();
    CREATE_ENTITY_WITH_A_COMPONENT();

    Component *component = ECS::GetComponent(entityId, COMPONENT_A_ID);
    ASSERT_TRUE(component != nullptr);
    ASSERT_EQ(component->id, COMPONENT_A_ID);

    Component *nonExistComponent = ECS::GetComponent(entityId, COMPONENT_B_ID);
    ASSERT_TRUE(nonExistComponent == nullptr);
}

TEST_F(ECSTest, EntityIdIsInSystemListIfMatch)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_AB_COMPONENTS();

    auto pSystemData = ECSAssert::GetSystemData(id, systemId);
    ASSERT_FALSE(ECSAssert::IsEntityInSystemMatchEntities(id, systemId, entityId));

    ECS::Update(ECS_TEST_DELTA_TIME);

    ASSERT_TRUE(ECSAssert::IsEntityInSystemMatchEntities(id, systemId, entityId));
}

TEST_F(ECSTest, CreateEntityMatchSystem)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);

    ASSERT_EQ(TestSystem::numberOfObjects, 1);
    ASSERT_EQ(TestSystem::initialCallCount, 0);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 0);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    CREATE_ENTITY_WITH_AB_COMPONENTS();
    ASSERT_EQ(TestSystem::initialCallCount, 0);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 0);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECSAssert::AssertNumberOfEntities(1);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 0);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 1);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 2);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::DestroyEntity(entityId);
    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 3);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 1);
}

TEST_F(ECSTest, UpdateComponentAWithTheSystem)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(AddComponentASystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_A_COMPONENT(5);

    Component *pComponent = ECS::GetComponent(entityId, COMPONENT_A_ID);
    ComponentA *pData = (ComponentA *)pComponent->pData;

    ECS::Update(ECS_TEST_DELTA_TIME);
    EXPECT_EQ(pData->a, 5);

    ECS::Update(ECS_TEST_DELTA_TIME);
    EXPECT_EQ(pData->a, 6);

    ECS::Update(ECS_TEST_DELTA_TIME);
    EXPECT_EQ(pData->a, 7);
}

TEST_F(ECSTest, EntityIsDeactivate)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_A_COMPONENT(5);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 0);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 1);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::ModifyEntityStatus(entityId, TRUE); // no effect
    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 2);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::ModifyEntityStatus(entityId, FALSE);
    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 3);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 3);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::ModifyEntityStatus(entityId, TRUE);
    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 3);
    ASSERT_EQ(TestSystem::resumeCallCount, 1);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 4);
    ASSERT_EQ(TestSystem::resumeCallCount, 1);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);
}

TEST_F(ECSTest, ComponentIsDeactivate)
{
    SINGLE_ECS_SETUP();
    SYSTEM_SETUP(TestSystem, COMPONENT_A_ID);
    CREATE_ENTITY_WITH_A_COMPONENT(5);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 0);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 1);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::ModifyComponentStatus(entityId, COMPONENT_A_ID, TRUE); // no effect
    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 0);
    ASSERT_EQ(TestSystem::updateCallCount, 2);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::ModifyComponentStatus(entityId, COMPONENT_A_ID, FALSE);
    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 3);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 3);
    ASSERT_EQ(TestSystem::resumeCallCount, 0);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::ModifyComponentStatus(entityId, COMPONENT_A_ID, TRUE);
    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 3);
    ASSERT_EQ(TestSystem::resumeCallCount, 1);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);

    ECS::Update(ECS_TEST_DELTA_TIME);
    ASSERT_EQ(TestSystem::initialCallCount, 1);
    ASSERT_EQ(TestSystem::suspendCallCount, 1);
    ASSERT_EQ(TestSystem::updateCallCount, 4);
    ASSERT_EQ(TestSystem::resumeCallCount, 1);
    ASSERT_EQ(TestSystem::shutdownCallCount, 0);
}
#endif // defined(RPP_PLATFORM_WINDOWS)