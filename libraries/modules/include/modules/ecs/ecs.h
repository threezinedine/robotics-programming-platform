#pragma once
#include "core/core.h"
#include "entity.h"
#include "component.h"
#include "system.h"
#include "modules/storage.h"

namespace rpp
{
    namespace internal
    {
    } // namespace internal

    typedef u32 ECSId; ///< The type used for ECS system IDs.

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
     * ECS::RegisterSystem(CreateScope<RenderingSystem>(), renderingSystemRequirements, 2);
     *
     * ComponentId inputSystemRequirements[] = {CLICKABLE_COMPONENT_ID};
     * ECS::RegisterSystem(CreateScope<InputSystem>(), inputSystemRequirements, 1);
     *
     * ComponentId collisionSystemRequirements[] = {GEOMETRY_COMPONENT_ID};
     * ECS::RegisterSystem(CreateScope<CollisionSystem>(), collisionSystemRequirements, 1);
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
    private:
        struct ECSData
        {
            struct SystemData
            {
                ComponentId *pRequiredComponents; ///< The list of component IDs which are required by the system.
                u32 numberOfRequiredComponents;   ///< The number of required components in the list.
                Scope<System> system;             ///< The user-custom system.
            };

            Scope<Storage<Entity>> entityStorage; ///< The storage for entities.
            Scope<List<SystemData>> systems;      ///< The list of systems.
        };

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
         * Create a new entity in the current ECS system instance.
         *
         * @param ppComponents The list of components to add to the entity. The data of the components will be copied into the heap memory.
         *      The memory of the component data must be deallocated manually via the ECS system. So the data for intializing the components
         *      can be delated after this method returns or using the heap memory directly.
         * @param numberOfComponents The number of components in the list.
         *
         * @return The ID of the created entity. Returns `INVALID_ID` if the entity could not be created.
         */
        static EntityId CreateEntity(Component **ppComponents, u32 numberOfComponents);

        /**
         * Destroy an entity in the current ECS system instance.
         *
         * @param entityId The ID of the entity to destroy.
         */
        static void DestroyEntity(EntityId entityId);

        /**
         * Retrieve the entity by its ID in the current ECS system instance.
         * @param entityId The ID of the entity to retrieve.
         * @return The pointer to the entity. Returns `nullptr` if the entity does not exist.
         */
        static Entity *GetEntity(EntityId entityId);

    public:
        /**
         * Attach a user-custom system into the current ECS system instance. The system will process all the entities
         * which have the required components each frame when `Update` is called.
         *
         * @param system The user-custom system to attach into the ECS system instance. The ECS system instance will take the ownership of the system.
         * @param pRequiredComponents The list of component IDs which are required by the system.
         * @param numberOfRequiredComponents The number of required components in the list.
         */
        void RegisterSystem(Scope<System> system, ComponentId *pRequiredComponents, u32 numberOfRequiredComponents);

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
