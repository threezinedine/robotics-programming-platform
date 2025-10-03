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

        auto DeallocateECS = [](ECSData *data)
        {
            RPP_ASSERT(data != nullptr);

            data->entityStorage.reset();
            data->systemStorage.reset();

            RPP_DELETE(data);
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
            RPP_DELETE(systemData->pSystem);

            RPP_DELETE(systemData);
        };

        ecsData->systemStorage = CreateScope<Storage<ECSData::SystemData>>(DeallocateSystemData);

        // reset dirty lists
        ecsData->dirtyEntities.Clear();
        RPP_ASSERT(ecsData->dirtyEntities.Size() == 0);
        ecsData->dirtyComponents.Clear();
        RPP_ASSERT(ecsData->dirtyComponents.Size() == 0);
        ecsData->dirtySystems.Clear();
        RPP_ASSERT(ecsData->dirtySystems.Size() == 0);

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

        currentEcs->dirtyEntities.Push(dirtyEntity);

        return entityId;
    }

    void ECS::DestroyEntity(EntityId entityId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        ECSData::DirtyEntity dirtyEntity;
        dirtyEntity.operation = ECSData::Operation::DELETE;
        dirtyEntity.entityId = entityId;

        currentEcs->dirtyEntities.Push(dirtyEntity);
    }

    Entity *ECS::GetEntity(EntityId entityId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        return currentEcs->entityStorage->Get(entityId);
    }

    Component *ECS::GetComponent(EntityId entityId, ComponentId componentId)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);
        RPP_ASSERT(componentId < MAX_NUMBER_OF_COMPONENTS);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        Entity *entity = currentEcs->entityStorage->Get(entityId);
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

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        ECS::ECSData::DirtyEntity dirtyEntity;
        dirtyEntity.operation = ECSData::Operation::CHANGE_STATE;
        dirtyEntity.entityId = entityId;
        currentEcs->dirtyEntities.Push(dirtyEntity);
    }

    void ECS::ModifyComponentStatus(EntityId entityId, ComponentId componentId, b8 isActive)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        ECS::ECSData::DirtyComponent dirtyComponent;
        dirtyComponent.operation = ECSData::Operation::CHANGE_STATE;
        dirtyComponent.entityId = entityId;
        dirtyComponent.componentId = componentId;
        dirtyComponent.isActive = isActive;

        currentEcs->dirtyComponents.Push(dirtyComponent);
    }

    void ECS::ModifySystemStatus(u32 systemId, b8 isActive)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);
        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        ECSData::SystemData *systemData = currentEcs->systemStorage->Get(systemId);
        RPP_ASSERT(systemData != nullptr);

        ECSData::DirtySystem dirtySystem;
        dirtySystem.operation = ECSData::Operation::CHANGE_STATE;
        dirtySystem.systemId = systemId;
        dirtySystem.isActive = isActive;

        currentEcs->dirtySystems.Push(dirtySystem);
    }

    u32 ECS::RegisterSystem(System *system, ComponentId *pRequiredComponents, u32 numberOfRequiredComponents)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);

        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);
        RPP_ASSERT(system != nullptr);

        SystemId systemId = currentEcs->systemStorage->Create();
        ECSData::SystemData *currentSystemData = currentEcs->systemStorage->Get(systemId);
        RPP_ASSERT(currentSystemData != nullptr);

        currentSystemData->pRequiredComponents = (ComponentId *)RPP_MALLOC(sizeof(ComponentId) * numberOfRequiredComponents);

        for (u32 componentIndex = 0; componentIndex < numberOfRequiredComponents; ++componentIndex)
        {
            currentSystemData->pRequiredComponents[componentIndex] = pRequiredComponents[componentIndex];
        }

        currentSystemData->numberOfRequiredComponents = numberOfRequiredComponents;
        currentSystemData->isActive = TRUE;
        currentSystemData->pSystem = system;

        return systemId;
    }

    void ECS::Update(f32 deltaTime)
    {
        RPP_ASSERT(s_ecsStorage != nullptr);
        RPP_ASSERT(s_currentEcsIndex != INVALID_ID);
        ECSData *currentEcs = s_ecsStorage->Get(s_currentEcsIndex);
        RPP_ASSERT(currentEcs != nullptr);

        // process dirty systems
        while (!currentEcs->dirtySystems.Empty())
        {
            ECSData::DirtySystem &dirtySystem = currentEcs->dirtySystems.Front();
            ECSData::SystemData *systemData = currentEcs->systemStorage->Get(dirtySystem.systemId);
            RPP_ASSERT(systemData != nullptr);

            if (dirtySystem.operation == ECSData::Operation::CHANGE_STATE)
            {
                systemData->isActive = dirtySystem.isActive;
            }

            currentEcs->dirtySystems.Pop();
        }

        // process dirty entities
        while (!currentEcs->dirtyEntities.Empty())
        {
            ECSData::DirtyEntity &dirtyEntity = currentEcs->dirtyEntities.Front();
            Entity *entity = currentEcs->entityStorage->Get(dirtyEntity.entityId);
            RPP_ASSERT(entity != nullptr);

            if (dirtyEntity.operation == ECSData::Operation::CHANGE_STATE)
            {
                entity->isActive = !entity->isActive;
            }
            else if (dirtyEntity.operation == ECSData::Operation::DELETE)
            {
                currentEcs->entityStorage->Free(dirtyEntity.entityId);
            }
            else if (dirtyEntity.operation == ECSData::Operation::CREATE)
            {
                // TODO: implement internal update
            }

            currentEcs->dirtyEntities.Pop();
        }

        // process dirty components
        while (!currentEcs->dirtyComponents.Empty())
        {
            ECSData::DirtyComponent &dirtyComponent = currentEcs->dirtyComponents.Front();
            Entity *entity = currentEcs->entityStorage->Get(dirtyComponent.entityId);

            if (entity == nullptr) ///< the entity is deleted above
            {
                currentEcs->dirtyComponents.Pop();
                continue;
            }

            ComponentId componentId = dirtyComponent.componentId;
            Component *component = entity->ppComponents[entity->componentIds[componentId]];
            RPP_ASSERT(component != nullptr);

            if (dirtyComponent.operation == ECSData::Operation::CHANGE_STATE)
            {
                component->isActive = dirtyComponent.isActive;
            }

            currentEcs->dirtyComponents.Pop();
        }

        // after the update, all the dirty entities, components, systems will be processed and the dirty lists will be empty.
        RPP_ASSERT(currentEcs->dirtyEntities.Size() == 0);
        RPP_ASSERT(currentEcs->dirtyComponents.Size() == 0);
        RPP_ASSERT(currentEcs->dirtySystems.Size() == 0);
    }
} // namespace rpp
