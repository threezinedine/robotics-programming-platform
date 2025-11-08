#include "core/singleton_manager.h"
#include "core/assertions.h"

namespace rpp
{
    Scope<Array<SingletonEntry>> SingletonManager::s_singletonEntries = nullptr;

    b8 SingletonManager::Initialize()
    {
        RPP_ASSERT(s_singletonEntries == nullptr);

        s_singletonEntries = CreateScope<Array<SingletonEntry>>();

        return TRUE;
    }

    void SingletonManager::Shutdown()
    {
        RPP_ASSERT(s_singletonEntries != nullptr);

        u32 size = s_singletonEntries->Size();
        for (i32 i = i32(size - 1); i >= 0; --i)
        {
            SingletonEntry &entry = s_singletonEntries->operator[](i);
            if (entry.destroyFunc)
            {
                entry.destroyFunc(entry);
            }
        }

        s_singletonEntries.reset();
    }

    void SingletonManager::RegisterSingleton(const String &name, void *instance, SingletonDestroyFunc destroyFunc)
    {
        s_singletonEntries->Push(SingletonEntry{name, instance, destroyFunc});
    }
} // namespace rpp
