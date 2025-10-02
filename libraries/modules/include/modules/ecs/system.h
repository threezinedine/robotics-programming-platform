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
         */
        void Initial(ECSId ecsId);

        /**
         * Be called each frame to update the system.
         *
         * The default implementation does nothing. The derived class can override this method to perform the update.
         * @param ecsId The ID of the ECS system instance.
         * @param deltaTime The time elapsed since the last frame in seconds.
         */
        void Update(ECSId ecsId, f32 deltaTime);

        /**
         * Be called each frame to render the system.
         *
         * The default implementation does nothing. The derived class can override this method to perform the rendering.
         * @param ecsId The ID of the ECS system instance.
         * @param deltaTime The time elapsed since the last frame in seconds.
         */
        void Shutdown(ECSId ecsId);

    protected:
        /**
         * The implementation of the Initial method. The derived class can override this method to perform the initialization.
         * @param ecsId The ID of the ECS system instance.
         */
        virtual void InitialImpl(ECSId ecsId);

        /**
         * The implementation of the Update method. The derived class can override this method to perform the update.
         * @param ecsId The ID of the ECS system instance.
         * @param deltaTime The time elapsed since the last frame in seconds.
         */
        virtual void UpdateImpl(ECSId ecsId, f32 deltaTime);

        /**
         * The implementation of the Shutdown method. The derived class can override this method to perform the shutdown.
         * @param ecsId The ID of the ECS system instance.
         */
        virtual void ShutdownImpl(ECSId ecsId);
    };
} // namespace rpp
