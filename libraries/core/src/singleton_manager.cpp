#include "core/singleton_manager.h"

namespace rpp
{
    Array<SingletonEntry> SingletonManager::s_singletonEntries;

    b8 SingletonManager::Initialize()
    {
        return TRUE;
    }

    void SingletonManager::Shutdown()
    {
        u32 size = s_singletonEntries.Size();
        for (i32 i = i32(size - 1); i >= 0; --i)
        {
            SingletonEntry &entry = s_singletonEntries[i];
            if (entry.destroyFunc)
            {
                entry.destroyFunc(entry);
            }
        }

        s_singletonEntries.Clear();
    }

    void SingletonManager::RegisterSingleton(const String &name, void *instance, SingletonDestroyFunc destroyFunc)
    {
        SingletonEntry entry;
        entry.name = name;
        entry.instance = instance;
        entry.destroyFunc = destroyFunc;

        s_singletonEntries.Push(entry);
    }
} // namespace rpp
