#pragma once
#include "core/core.h"
#include "entity.h"
#include "component.h"
#include "system.h"
#include "modules/storage.h"
#include "type.h"

class ECSAssert; ///< Used only for testing purpose.

namespace rpp
{

    namespace internal
    {
    } // namespace internal

    /**
     * The ECS (Entity-Component-System) class is responsible for managing entities, components, and systems.
     * It provides methods to create and destroy entities, add and remove components, and update systems
     * each frame. Each entity with a matching set of components will be processed by the corresponding systems.
     * The ECS class uses a storage mechanism to efficiently manage and access entities and components.
     *
     * @example
     * ```cpp
     *
     * ECSId ecsId = ECS::Create();
     *
     * ECS::Activate(ecsId);
     *
     * #define GEOMETRY_COMPONENT_ID 1 // If have this component, entity will be passed into colision system.
     * #define TEXTURE_COMPONENT_ID 2 // If have this component and `GEOMETRY_COMPONENT_ID`, entity will be passed into rendering system.
     * #define CLICKABLE_COMPONENT_ID 3 // If have this component, entity will be passed into input system.
     *
     * ComponentId renderingSystemRequirements[] = {GEOMETRY_COMPONENT_ID, TEXTURE_COMPONENT_ID};
     * ECS::RegisterSystem(RPP_NEW(RenderingSystem), renderingSystemRequirements, 2);
     *
     * ComponentId inputSystemRequirements[] = {CLICKABLE_COMPONENT_ID};
     * ECS::RegisterSystem(RPP_NEW(InputSystem), inputSystemRequirements, 1);
     *
     * ComponentId collisionSystemRequirements[] = {GEOMETRY_COMPONENT_ID};
     * ECS::RegisterSystem(RPP_NEW(CollisionSystem), collisionSystemRequirements, 1);
     *
     * geometryData = {...}; // The user-defined data for the geometry component.
     * Component geometryComponent = {GEOMETRY_COMPONENT_ID, &geometryData, sizeof(geometryData)};
     *
     * textureData = {...}; // The user-defined data for the texture component.
     * Component textureComponent = {TEXTURE_COMPONENT_ID, &textureData, sizeof(textureData)};
     *
     * clickableData = {...}; // The user-defined data for the clickable component.
     * Component clickableComponent = {CLICKABLE_COMPONENT_ID, &clickableData, sizeof(clickableData)};
     *
     * Component* components[] = {&geometryComponent, &textureComponent, &clickableComponent};
     * EntityId entityId = ECS::CreateEntity(components, 3);
     *
     * while (true)
     * {
     *      ECS::Update(deltaTime); // Update all the systems in the ECS instance.
     * }
     *
     * ECS::Destroy(ecsId);
     *
     * ```
     */
    class ECS
    {
    public:
        friend class ECSAssert; ///< Used only for testing purpose.

    private:
        struct ECSData
        {
            struct SystemData
            {
                ComponentId *pRequiredComponents; ///< The list of component IDs which are required by the system.
                u32 numberOfRequiredComponents;   ///< The number of required components in the list.
                System *pSystem;                  ///< The user-custom system.
                b8 isActive;                      ///< If not active, the system will not be updated each frame.
            };

            Scope<Storage<Entity>> entityStorage;     ///< The storage for entities.
            Scope<Storage<SystemData>> systemStorage; ///< The storage for systems.

            enum class Operation
            {
                CREATE,      ///< The entity/component/system needs to be added.
                DELETE,      ///< The entity/component/system needs to be removed.
                CHANGE_STATE ///< The entity/component/system needs to be modified (like activate/deactivate, add/remove component, etc.).
            }; ///< The operation to perform on the entity/component/system.

            struct DirtyEntity
            {
                Operation operation; ///< The operation to perform on the entity.
                EntityId entityId;   ///< The ID of the entity which needs to be added/removed or modified.
                b8 isActive;         ///< The new active status of the entity (if the operation is CHANGE_STATE or be ignored).
            };
            Queue<DirtyEntity> dirtyEntities; ///< The list of entities which need to be added/removed or modified.

            struct DirtyComponent
            {
                Operation operation;     ///< The operation to perform on the component.
                EntityId entityId;       ///< The ID of the entity which has the component to be modified.
                ComponentId componentId; ///< The ID of the component to be modified.
                b8 isActive;             ///< The new active status of the component (if the operation is CHANGE_STATE or be ignored).
            };
            Queue<DirtyComponent> dirtyComponents; ///< The list of components which need to be modified. Has no CREATE or DELETE operation.

            struct DirtySystem
            {
                Operation operation; ///< The operation to perform on the system.
                u32 systemId;        ///< The ID of the system to be modified.
                b8 isActive;         ///< The new active status of the system (if the operation is CHANGE_STATE or be ignored).
            };
            Queue<DirtySystem> dirtySystems; ///< The list of systems which need to be modified. Has no CREATE operation.
        };

    private:
        /**
         * Used internally for testing whether an entity matches the required components of a system.
         *
         * @param pEntity The entity to check.
         * @param pSystemData The system data which contains the required components.
         */
        static b8 IsEntityMatchSystem(Entity *pEntity, ECSData::SystemData *pSystemData);

    public:
        /**
         * Initialize the ECS system. This must be called before any other methods.
         * @note This is called automatically when the module is loaded.
         */
        static void Initialize();

        /**
         * Shutdown the ECS system. This must be called when the ECS system is no longer needed.
         */
        static void Shutdown();

    public:
        /**
         * Create the ECS system instance (for each graphic session).
         */
        static ECSId Create();

        /**
         * Destroy the ECS system instance.
         * @param ecsId The ID of the ECS system instance to destroy.
         */
        static void Destroy(ECSId ecsId);

        /**
         * Assign the new ECS system instance to the current graphic session.
         */
        static void Activate(ECSId ecsId);

    public:
        /**
         * Retrieve the entity by its ID in the current ECS system instance.
         * @param entityId The ID of the entity to retrieve.
         * @return The pointer to the entity. Returns `nullptr` if the entity does not exist.
         */
        static Entity *GetEntity(EntityId entityId);

        /**
         * Retrieve the component by its ID from an entity in the current ECS system instance.
         * @param entityId The ID of the entity which has the component to retrieve.
         * @param componentId The ID of the component to retrieve.
         * @return The pointer to the component. Returns `nullptr` if the entity or the component does not exist.
         */
        static Component *GetComponent(EntityId entityId, ComponentId componentId);

        // Delay operations like activating/deactivating entities, removing components will be applied after all the systems are updated.
    public:
        /**
         * Create a new entity in the current ECS system instance.
         *
         * @param ppComponents The list of components to add to the entity. The data of the components will be copied into the heap memory.
         *      The memory of the component data must be deallocated manually via the ECS system. So the data for intializing the components
         *      can be delated after this method returns or using the heap memory directly.
         * @param numberOfComponents The number of components in the list.
         *
         * @return The ID of the created entity. Returns `INVALID_ID` if the entity could not be created.
         *
         * @note actually add the entity after call `Update` method of the ECS class.
         */
        static EntityId CreateEntity(Component **ppComponents, u32 numberOfComponents);

        /**
         * Destroy an entity in the current ECS system instance.
         *
         * @param entityId The ID of the entity to destroy.
         */
        static void DestroyEntity(EntityId entityId);

        /**
         * Delay operations like activating/deactivating entities.
         *
         * @param entityId The ID of the entity to modify.
         * @param isActive The new active status of the entity.
         *
         * @note actually modify the entity after call `Update` method of the ECS class.
         */
        static void ModifyEntityStatus(EntityId entityId, b8 isActive);

        /**
         * Delay operations like activating/deactivating components.
         * @param entityId The ID of the entity which has the component to modify.
         * @param componentId The ID of the component to modify.
         * @param isActive The new active status of the component.
         * @note actually modify the component after call `Update` method of the ECS class.
         */
        static void ModifyComponentStatus(EntityId entityId, ComponentId componentId, b8 isActive);

        /**
         * Delay operations like activating/deactivating systems.
         * @param systemId The ID of the system to modify.
         * @param isActive The new active status of the system.
         * @note actually modify the system after call `Update` method of the ECS class.
         */
        static void ModifySystemStatus(u32 systemId, b8 isActive);

    public:
        /**
         * Attach a user-custom system into the current ECS system instance. The system will process all the entities
         * which have the required components each frame when `Update` is called.
         *
         * @param system The user-custom system to attach into the ECS system instance. The ECS system instance will take the ownership of the system.
         * @param pRequiredComponents The list of component IDs which are required by the system.
         * @param numberOfRequiredComponents The number of required components in the list.
         */
        static u32 RegisterSystem(System *system, ComponentId *pRequiredComponents, u32 numberOfRequiredComponents);

        /**
         * Update all the systems in the current ECS system instance. Each system will process all the entities
         * which have the required components.
         *
         * @param deltaTime The time elapsed since the last update call.
         *
         * @note All the logic which can affect the entities or components' status (like active, inactive, add, remove, etc.) will be
         * applied after all the systems are updated (after systems are processed and be at the end of this method).
         */
        static void Update(f32 deltaTime);

    private:
        static Scope<Storage<ECSData>> s_ecsStorage; ///< The storage for the ECS system instances.
        static ECSId s_currentEcsIndex;              ///< The index of the current ECS system instance.
    };
} // namespace rpp
