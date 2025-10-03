#pragma once
#include "type.h"

namespace rpp
{
    /**
     * All other systems must inherit from this class and implement the pure virtual methods.
     * The system will be managed by the ECS class and will be called each frame to process
     * the entities which have the required components.
     */
    class System
    {
    public:
        virtual ~System() = default;

        /**
         * Be called each time when an entity which matches the system's required components is created or activated.
         *
         * The default implementation does nothing. The derived class can override this method to perform the initialization.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being initialized.
         */
        void Initial(ECSId ecsId, EntityId entityId);

        /**
         * Be called each time when an entity or its matched components are deactivated.
         *
         * The default implementation does nothing. The derived class can override this method to perform the suspension.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being suspended.
         */
        void Suspend(ECSId ecsId, EntityId entityId);

        /**
         * Be called each frame to update the system.
         *
         * The default implementation does nothing. The derived class can override this method to perform the update.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being updated.
         * @param deltaTime The time elapsed since the last frame in seconds.
         */
        void Update(ECSId ecsId, EntityId entityId, f32 deltaTime);

        /**
         * Be called each time when an entity or its matched components are re-activated.
         * The default implementation does nothing. The derived class can override this method to perform the resumption.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being resumed.
         */
        void Resume(ECSId ecsId, EntityId entityId);

        /**
         * Be called each frame to render the system.
         *
         * The default implementation does nothing. The derived class can override this method to perform the rendering.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being rendered.
         */
        void Shutdown(ECSId ecsId, EntityId entityId);

    protected:
        /**
         * The implementation of the Initial method. The derived class can override this method to perform the initialization.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being initialized.
         */
        virtual void InitialImpl(ECSId ecsId, EntityId entityId);

        /**
         * The implementation of the Suspend method. The derived class can override this method to perform the suspension.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being suspended.
         */
        virtual void SuspendImpl(ECSId ecsId, EntityId entityId);

        /**
         * The implementation of the Update method. The derived class can override this method to perform the update.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being updated.
         * @param deltaTime The time elapsed since the last frame in seconds.
         */
        virtual void UpdateImpl(ECSId ecsId, EntityId entityId, f32 deltaTime);

        /**
         * The implementation of the Resume method. The derived class can override this method to perform the resumption.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being resumed.
         */
        virtual void ResumeImpl(ECSId ecsId, EntityId entityId);

        /**
         * The implementation of the Shutdown method. The derived class can override this method to perform the shutdown.
         * @param ecsId The ID of the ECS system instance.
         * @param entityId The ID of the entity being shut down.
         */
        virtual void ShutdownImpl(ECSId ecsId, EntityId entityId);
    };
} // namespace rpp
