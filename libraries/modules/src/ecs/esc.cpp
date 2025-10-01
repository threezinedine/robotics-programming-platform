#include "modules/ecs/ecs.h"

namespace rpp
{
    Scope<Storage<ECS::ECSData>> ECS::s_ecsStorage = nullptr;
    ECSId ECS::s_currentEcsIndex = INVALID_ID;

    void ECS::Initialize()
    {
        RPP_ASSERT(s_ecsStorage == nullptr);
        RPP_ASSERT(s_currentEcsIndex == INVALID_ID);

        auto DeallocateECS = [](ECSData *data)
        {
            RPP_ASSERT(data != nullptr);

            data->entityStorage.reset();

            RPP_DELETE(data);
        };

        s_ecsStorage = CreateScope<Storage<ECSData>>(DeallocateECS);
    }

    void ECS::Shutdown()
    {
        RPP_ASSERT(s_ecsStorage != nullptr);

        s_ecsStorage.reset();
    }

    ECSId ECS::Create()
    {
        RPP_ASSERT(s_ecsStorage != nullptr);

        u32 ecsId = s_ecsStorage->Create();

        ECSData *ecsData = s_ecsStorage->Get(ecsId);
        RPP_ASSERT(ecsData != nullptr);

        auto DeallocateEntityData = [](Entity *entity)
        {
            RPP_ASSERT(entity != nullptr);

            for (u32 componentIndex = 0; componentIndex < entity->numberOfComponents; ++componentIndex)
            {
                RPP_DELETE(entity->ppComponents[componentIndex]->pData);
                RPP_DELETE(entity->ppComponents[componentIndex]);
            }

            RPP_DELETE(entity->ppComponents);

            RPP_DELETE(entity);
        };

        ecsData->entityStorage = CreateScope<Storage<Entity>>(DeallocateEntityData);

        auto DeallocateSystemData = [](ECSData::SystemData *systemData)
        {
            RPP_ASSERT(systemData != nullptr);

            RPP_DELETE(systemData->pRequiredComponents);
            systemData->system.reset();

            RPP_DELETE(systemData);
        };

        ecsData->systems = CreateScope<List<ECSData::SystemData>>(DeallocateSystemData);

        return ecsId;
    }

    void ECS::Destroy(ECSId ecsId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(ecsId != INVALID_ID);
        RPP_ASSERT(s_ecsStorage->Get(ecsId) != nullptr);

        s_ecsStorage->Free(ecsId);
        if (s_currentEcsIndex == ecsId)
        {
            s_currentEcsIndex = INVALID_ID;
        }
    }

    void ECS::Activate(ECSId ecsId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(ecsId != INVALID_ID);
        RPP_ASSERT(s_ecsStorage->Get(ecsId) != nullptr);

        s_currentEcsIndex = ecsId;
    }

    EntityId ECS::CreateEntity(Component **ppComponents, u32 numberOfComponents)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        EntityId entityId = currentEcs->entityStorage->Create();
        RPP_ASSERT(entityId != INVALID_ID);
        Entity *entity = currentEcs->entityStorage->Get(entityId);

        // copy the components into the entity
        entity->id = entityId;
        entity->isActive = TRUE;
        entity->numberOfComponents = numberOfComponents;

        entity->ppComponents = (Component **)RPP_MALLOC(sizeof(Component *) * numberOfComponents);

        for (u32 componentIndex = 0; componentIndex < numberOfComponents; ++componentIndex)
        {
            Component **pSrcComponent = &ppComponents[componentIndex];
            u32 componentSize = (*pSrcComponent)->size;

            Component **pDstComponent = &entity->ppComponents[componentIndex];

            *pDstComponent = (Component *)RPP_MALLOC(sizeof(Component));
            (*pDstComponent)->id = (*pSrcComponent)->id;
            (*pDstComponent)->isActive = (*pSrcComponent)->isActive;
            (*pDstComponent)->size = componentSize;
            (*pDstComponent)->pData = RPP_MALLOC(componentSize);

            memcpy((*pDstComponent)->pData, (*pSrcComponent)->pData, componentSize);
        }
    }

    void ECS::DestroyEntity(EntityId entityId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        currentEcs->entityStorage->Free(entityId);
    }

    Entity *ECS::GetEntity(EntityId entityId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        return currentEcs->entityStorage->Get(entityId);
    }

    void ECS::RegisterSystem(Scope<System> system, ComponentId *pRequiredComponents, u32 numberOfRequiredComponents)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);
        RPP_ASSERT(system != nullptr);

        ECSData::SystemData systemData;
        currentEcs->systems->Push(systemData);

        u32 currentSystemIndex = currentEcs->systems->Size() - 1;
        ECSData::SystemData &currentSystemData = (*currentEcs->systems)[currentSystemIndex];

        currentSystemData.pRequiredComponents = (ComponentId *)RPP_MALLOC(sizeof(ComponentId) * numberOfRequiredComponents);
        currentSystemData.numberOfRequiredComponents = numberOfRequiredComponents;
        currentSystemData.system = std::move(system);
    }

    void ECS::Update(f32 deltaTime)
    {
        // Currently nothing to do here.
    }
} // namespace rpp
