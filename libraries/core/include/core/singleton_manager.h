#pragma once
#include "platforms/platforms.h"

namespace rpp
{
    /**
     * @brief The main solidity object (singleton manager) that manages all singleton objects.
     *
     * @example
     * ```cpp
     * class TestManager ... // Define the singleton class
     *
     * SingletonManager::Initialize(); // Initialize the singleton manager
     *
     * TestManager::GetInstance()->DoSomething(); // the singleton instance is created at the first time GetInstance is called
     *
     * SingletonManager::Shutdown(); // Shutdown the singleton instance
     *
     * ```
     */
    class SingletonManager
    {
    public:
        /**
         * @brief Starting the manager and resource allocation.
         */
        static b8 Initialize();

        /**
         * @brief Shutdown the manager and release all resources.
         */
        static void Shutdown();
    };
} // namespace rpp
