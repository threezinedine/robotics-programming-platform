#include "core/threading/thread.h"
#include "core/assertions.h"
#include <cstring>

#if defined(RPP_PLATFORM_WINDOWS) || defined(RPP_PLATFORM_LINUX)
#include <thread>

namespace rpp
{
    Scope<Storage<Thread::ThreadData>> Thread::s_threads = nullptr;

    void Thread::Initialize()
    {
        RPP_ASSERT(s_threads == nullptr);

        auto Deallocator = [](Thread::ThreadData *pData)
        {
            RPP_ASSERT(pData != nullptr);

            if (pData->pData != nullptr)
            {
                RPP_FREE(pData->pData);
                pData->pData = nullptr;
            }

            if (pData->pHandle != nullptr)
            {
                std::thread *pThread = static_cast<std::thread *>(pData->pHandle);
                if (pThread->joinable())
                {
                    pThread->join();
                }
                RPP_DELETE(pThread);
                pData->pHandle = nullptr;
            }

            RPP_DELETE(pData);
        };

        s_threads = CreateScope<Storage<ThreadData>>(Deallocator);
    }

    void Thread::Shutdown()
    {
        RPP_ASSERT(s_threads != nullptr);
        s_threads.reset();
    }

    ThreadId Thread::Create(ThreadEntry entry, void *pParam, u32 paramSize)
    {
        RPP_ASSERT(s_threads != nullptr);

        ThreadId threadId = s_threads->Create();
        ThreadData *pData = s_threads->Get(threadId);
        RPP_ASSERT(pData != nullptr);

        if (pParam != nullptr && paramSize > 0)
        {
            pData->dataSize = paramSize;
            pData->pData = RPP_MALLOC(paramSize);
            std::memcpy(pData->pData, pParam, paramSize);
        }
        else
        {
            pData->dataSize = 0;
            pData->pData = nullptr;
        }

        pData->entry = entry;
        pData->isRunning = FALSE;
        pData->pHandle = nullptr;

        return threadId;
    }

    void Thread::Destroy(ThreadId threadId)
    {
        RPP_ASSERT(s_threads != nullptr);

        s_threads->Free(threadId);
    }

    void Thread::Start(ThreadId threadId)
    {
        RPP_ASSERT(s_threads != nullptr);

        Thread::ThreadData *pData = s_threads->Get(threadId);
        RPP_ASSERT(pData != nullptr);
        RPP_ASSERT(!pData->isRunning);

        pData->isRunning = TRUE;
        std::thread *pThread = RPP_NEW(std::thread, pData->entry, pData->pData);
        pData->pHandle = pThread;
    }

    void Thread::Join(ThreadId threadId)
    {
        RPP_ASSERT(s_threads != nullptr);
        Thread::ThreadData *data = s_threads->Get(threadId);
        RPP_ASSERT(data != nullptr);

        if (!data->isRunning)
        {
            return; // Thread is not running
        }

        std::thread *pThread = static_cast<std::thread *>(data->pHandle);
        if (pThread != nullptr && pThread->joinable())
        {
            pThread->join();
        }
        data->isRunning = FALSE;
    }

    void Thread::Sleep(u32 milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
} // namespace rpp

#endif // RPP_PLATFORM_WINDOWS