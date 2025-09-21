#pragma once
#include "platforms/platforms.h"
#include "array.h"
#include "string.h"

namespace rpp
{
    struct SingletonEntry;

    /**
     * @brief Callback which will be called automatically when the singleton manager is shutdown.
     */
    typedef void (*SingletonDestroyFunc)(const SingletonEntry &);

    struct SingletonEntry
    {
        String name;                      ///< Name of the singleton object.
        void *instance;                   ///< Pointer to the singleton object instance.
        SingletonDestroyFunc destroyFunc; ///< Function to destroy the singleton object.

        SingletonEntry(const String& name, void* instance, SingletonDestroyFunc destroyFunc) 
            : name(name), instance(instance), destroyFunc(destroyFunc) 
		{
		}

        SingletonEntry(const SingletonEntry& other)
			: name(other.name), instance(other.instance), destroyFunc(other.destroyFunc)
        {
        }
    };

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

        /**
         * @brief Register a singleton object with the manager.
         * @param name Name of the singleton object.
         * @param instance Pointer to the singleton object instance.
         * @param destroyFunc Function to destroy the singleton object.
         */
        static void RegisterSingleton(const String &name, void *instance, SingletonDestroyFunc destroyFunc);

    private:
        static Array<SingletonEntry> s_singletonEntries; ///< Array of all registered singleton objects.
    };
} // namespace rpp

#define RPP_SINGLETON_DEFINE(classType) \
public:                                 \
    static classType *s_instsance;      \
    static classType *GetInstance();    \
                                        \
private:                                \
    classType();                        \
    ~classType();

#define RPP_SINGLETON_IMPLEMENT(classType)                                                             \
    classType *classType::s_instsance = nullptr;                                                       \
    classType *classType::GetInstance()                                                                \
    {                                                                                                  \
        if (s_instsance == nullptr)                                                                    \
        {                                                                                              \
            s_instsance = new classType();                                                             \
            SingletonManager::RegisterSingleton(                                                       \
                #classType,                                                                            \
                s_instsance,                                                                           \
                [](const SingletonEntry &entry) { delete static_cast<classType *>(entry.instance); }); \
        }                                                                                              \
        return s_instsance;                                                                            \
    }
