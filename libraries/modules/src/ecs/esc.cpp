#include "modules/ecs/ecs.h"

namespace rpp
{
    Scope<Storage<ECS::ECSData>> ECS::s_ecsStorage = nullptr;
    ECSId ECS::s_currentEcsIndex = INVALID_ID;

    b8 ECS::IsEntityMatchSystem(Entity *pEntity, ECSData::SystemData *pSystemData)
    {
        RPP_ASSERT(pEntity != nullptr);

        if (!pEntity->isActive)
        {
            return FALSE;
        }

        u32 systemRequiredComponentCount = pSystemData->numberOfRequiredComponents;
        u32 entityComponentCount = pEntity->numberOfComponents;

        for (u32 requirementIndex = 0; requirementIndex < systemRequiredComponentCount; ++requirementIndex)
        {
            b8 isRequirementMet = FALSE;
            ComponentId requiredComponentId = pSystemData->pRequiredComponents[requirementIndex];

            u32 componentIndex = pEntity->componentIds[requiredComponentId];
            if (componentIndex == u32(-1))
            {
                return FALSE;
            }

            if (!pEntity->ppComponents[componentIndex]->isActive)
            {
                return FALSE;
            }
        }

        return TRUE;
    }

    void ECS::Initialize()
    {
        RPP_ASSERT(s_ecsStorage == nullptr);
        RPP_ASSERT(s_currentEcsIndex == INVALID_ID);

        auto DeallocateECS = [](ECSData *pData)
        {
            RPP_ASSERT(pData != nullptr);

            pData->entityStorage.reset();
            pData->systemStorage.reset();

            RPP_DELETE(pData);
        };

        s_ecsStorage = CreateScope<Storage<ECSData>>(DeallocateECS);
    }

    void ECS::Shutdown()
    {
        RPP_ASSERT(s_ecsStorage != nullptr);

        s_ecsStorage.reset();
        s_currentEcsIndex = INVALID_ID;
    }

    ECSId ECS::Create()
    {
        RPP_ASSERT(s_ecsStorage != nullptr);

        u32 ecsId = s_ecsStorage->Create();

        ECSData *pEcsData = s_ecsStorage->Get(ecsId);
        RPP_ASSERT(pEcsData != nullptr);

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

        pEcsData->entityStorage = CreateScope<Storage<Entity>>(DeallocateEntityData);

        auto DeallocateSystemData = [](ECSData::SystemData *systemData)
        {
            RPP_ASSERT(systemData != nullptr);
            RPP_DELETE(systemData->pRequiredComponents);
            RPP_DELETE(systemData->pSystem);

            RPP_DELETE(systemData);
        };

        pEcsData->systemStorage = CreateScope<Storage<ECSData::SystemData>>(DeallocateSystemData);

        // reset dirty lists
        pEcsData->dirtyEntities.Clear();
        RPP_ASSERT(pEcsData->dirtyEntities.Size() == 0);
        pEcsData->dirtyComponents.Clear();
        RPP_ASSERT(pEcsData->dirtyComponents.Size() == 0);
        pEcsData->dirtySystems.Clear();
        RPP_ASSERT(pEcsData->dirtySystems.Size() == 0);

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

        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);

        EntityId entityId = pCurrentEcs->entityStorage->Create();
        RPP_ASSERT(entityId != INVALID_ID);
        Entity *entity = pCurrentEcs->entityStorage->Get(entityId);

        // copy the components into the entity
        entity->id = entityId;
        entity->isActive = TRUE;
        entity->numberOfComponents = numberOfComponents;

        entity->ppComponents = (Component **)RPP_MALLOC(sizeof(Component *) * numberOfComponents);

        memset(entity->componentIds, -1, sizeof(u32) * MAX_NUMBER_OF_COMPONENTS);

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

            entity->componentIds[(*pDstComponent)->id] = componentIndex;
        }

        ECSData::DirtyEntity dirtyEntity;
        dirtyEntity.operation = ECSData::Operation::CREATE;
        dirtyEntity.entityId = entityId;

        pCurrentEcs->dirtyEntities.Push(dirtyEntity);

        return entityId;
    }

    void ECS::DestroyEntity(EntityId entityId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);

        ECSData::DirtyEntity dirtyEntity;
        dirtyEntity.operation = ECSData::Operation::DELETE;
        dirtyEntity.entityId = entityId;

        pCurrentEcs->dirtyEntities.Push(dirtyEntity);
    }

    Entity *ECS::GetEntity(EntityId entityId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);

        return pCurrentEcs->entityStorage->Get(entityId);
    }

    Component *ECS::GetComponent(EntityId entityId, ComponentId componentId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);
        RPP_ASSERT(componentId < MAX_NUMBER_OF_COMPONENTS);

        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);

        Entity *entity = pCurrentEcs->entityStorage->Get(entityId);
        RPP_ASSERT(entity != nullptr);

        u32 componentIndex = entity->componentIds[componentId];

        if (componentIndex == u32(-1))
        {
            return nullptr;
        }

        return entity->ppComponents[componentIndex];
    }

    void ECS::ModifyEntityStatus(EntityId entityId, b8 isActive)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);

        ECS::ECSData::DirtyEntity dirtyEntity;
        dirtyEntity.operation = ECSData::Operation::CHANGE_STATE;
        dirtyEntity.entityId = entityId;
        pCurrentEcs->dirtyEntities.Push(dirtyEntity);
    }

    void ECS::ModifyComponentStatus(EntityId entityId, ComponentId componentId, b8 isActive)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);

        ECS::ECSData::DirtyComponent dirtyComponent;
        dirtyComponent.operation = ECSData::Operation::CHANGE_STATE;
        dirtyComponent.entityId = entityId;
        dirtyComponent.componentId = componentId;
        dirtyComponent.isActive = isActive;

        pCurrentEcs->dirtyComponents.Push(dirtyComponent);
    }

    void ECS::ModifySystemStatus(u32 systemId, b8 isActive)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);
        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);

        ECSData::SystemData *pSystemData = pCurrentEcs->systemStorage->Get(systemId);
        RPP_ASSERT(pSystemData != nullptr);

        ECSData::DirtySystem dirtySystem;
        dirtySystem.operation = ECSData::Operation::CHANGE_STATE;
        dirtySystem.systemId = systemId;
        dirtySystem.isActive = isActive;

        pCurrentEcs->dirtySystems.Push(dirtySystem);
    }

    u32 ECS::RegisterSystem(System *system, ComponentId *pRequiredComponents, u32 numberOfRequiredComponents)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);
        RPP_ASSERT(system != nullptr);

        SystemId systemId = pCurrentEcs->systemStorage->Create();
        ECSData::SystemData *pCurrentSystemData = pCurrentEcs->systemStorage->Get(systemId);
        RPP_ASSERT(pCurrentEcs->entityStorage->GetNumberOfElements() == 0); // all systems must be registered before any entity is created.
        RPP_ASSERT(pCurrentSystemData != nullptr);

        pCurrentSystemData->pRequiredComponents = (ComponentId *)RPP_MALLOC(sizeof(ComponentId) * numberOfRequiredComponents);

        for (u32 componentIndex = 0; componentIndex < numberOfRequiredComponents; ++componentIndex)
        {
            pCurrentSystemData->pRequiredComponents[componentIndex] = pRequiredComponents[componentIndex];
        }

        pCurrentSystemData->numberOfRequiredComponents = numberOfRequiredComponents;
        pCurrentSystemData->isActive = TRUE;
        pCurrentSystemData->pSystem = system;

        return systemId;
    }

    void ECS::Update(f32 deltaTime)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);
        ECSData *pCurrentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(pCurrentEcs != nullptr);

        // process dirty systems
        while (!pCurrentEcs->dirtySystems.Empty())
        {
            ECSData::DirtySystem &dirtySystem = pCurrentEcs->dirtySystems.Front();
            ECSData::SystemData *systemData = pCurrentEcs->systemStorage->Get(dirtySystem.systemId);
            RPP_ASSERT(systemData != nullptr);

            if (dirtySystem.operation == ECSData::Operation::CHANGE_STATE)
            {
                systemData->isActive = dirtySystem.isActive;
            }

            pCurrentEcs->dirtySystems.Pop();
        }

        // process dirty entities
        while (!pCurrentEcs->dirtyEntities.Empty())
        {
            ECSData::DirtyEntity &dirtyEntity = pCurrentEcs->dirtyEntities.Front();
            Entity *entity = pCurrentEcs->entityStorage->Get(dirtyEntity.entityId);
            RPP_ASSERT(entity != nullptr);

            if (dirtyEntity.operation == ECSData::Operation::CHANGE_STATE)
            {
                entity->isActive = !entity->isActive;
            }
            else if (dirtyEntity.operation == ECSData::Operation::DELETE)
            {
                pCurrentEcs->entityStorage->Free(dirtyEntity.entityId);
            }
            else if (dirtyEntity.operation == ECSData::Operation::CREATE)
            {
                u32 numberOfSystems = pCurrentEcs->systemStorage->GetNumberOfElements();
                for (u32 systemIndex = 0; systemIndex < numberOfSystems; ++systemIndex)
                {
                    ECSData::SystemData *systemData = pCurrentEcs->systemStorage->Get(systemIndex);
                    RPP_ASSERT(systemData != nullptr);

                    if (systemData->isActive && IsEntityMatchSystem(entity, systemData))
                    {
                        systemData->matchedEntities.Push(entity->id);
                    }
                }
            }

            pCurrentEcs->dirtyEntities.Pop();
        }

        // process dirty components
        while (!pCurrentEcs->dirtyComponents.Empty())
        {
            ECSData::DirtyComponent &dirtyComponent = pCurrentEcs->dirtyComponents.Front();
            Entity *entity = pCurrentEcs->entityStorage->Get(dirtyComponent.entityId);

            if (entity == nullptr) ///< the entity is deleted above
            {
                pCurrentEcs->dirtyComponents.Pop();
                continue;
            }

            ComponentId componentId = dirtyComponent.componentId;
            Component *component = entity->ppComponents[entity->componentIds[componentId]];
            RPP_ASSERT(component != nullptr);

            if (dirtyComponent.operation == ECSData::Operation::CHANGE_STATE)
            {
                component->isActive = dirtyComponent.isActive;
            }

            pCurrentEcs->dirtyComponents.Pop();
        }

        // after the update, all the dirty entities, components, systems will be processed and the dirty lists will be empty.
        RPP_ASSERT(pCurrentEcs->dirtyEntities.Size() == 0);
        RPP_ASSERT(pCurrentEcs->dirtyComponents.Size() == 0);
        RPP_ASSERT(pCurrentEcs->dirtySystems.Size() == 0);
    }
} // namespace rpp
