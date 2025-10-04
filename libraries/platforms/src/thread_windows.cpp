#include "platforms/threading/thread.h"
#include "platforms/memory.h"

#if defined(RPP_PLATFORM_WINDOWS)
#include <thread>

namespace rpp
{
    Thread::ThreadData *Thread::s_threadData[RPP_MAX_THREAD_COUNT];

    void Thread::Initialize()
    {
        memset(s_threadData, 0, sizeof(s_threadData));
    }

    void Thread::Shutdown()
    {
        u32 threadId = 0;
        while (threadId < RPP_MAX_THREAD_COUNT)
        {
            if (s_threadData[threadId] != nullptr)
            {
                Destroy(threadId);
            }
            threadId++;
        }
    }

    ThreadId Thread::Create(ThreadEntry entry, void *pParam, u32 paramSize)
    {
        ThreadId threadId = 0;

        while (threadId <= RPP_MAX_THREAD_COUNT)
        {
            if (threadId == RPP_MAX_THREAD_COUNT)
            {
                RPP_UNREACHABLE(); // Exceeded maximum thread count
            }

            Thread::ThreadData *data = s_threadData[threadId];

            if (data == nullptr)
            {
                break;
            }
            else
            {
                threadId++;
            }
        }

        Thread::ThreadData *data = RPP_NEW(Thread::ThreadData{});
        if (pParam != nullptr && paramSize > 0)
        {
            data->dataSize = paramSize;
            data->pData = RPP_MALLOC(paramSize);
            memcpy(data->pData, pParam, paramSize);
        }
        else
        {
            data->dataSize = 0;
            data->pData = nullptr;
        }

        data->entry = entry;
        data->isRunning = FALSE;
        data->pHandle = nullptr;

        s_threadData[threadId] = data;

        return threadId;
    }

    void Thread::Destroy(ThreadId threadId)
    {
        if (threadId >= RPP_MAX_THREAD_COUNT)
        {
            RPP_UNREACHABLE(); // Invalid thread ID
        }

        Thread::ThreadData *pData = s_threadData[threadId];

        if (pData == nullptr)
        {
            RPP_UNREACHABLE(); // Thread not found
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

        if (pData->pData != nullptr)
        {
            RPP_FREE(pData->pData);
            pData->pData = nullptr;
        }

        RPP_DELETE(pData);
        s_threadData[threadId] = nullptr;
    }

    void Thread::Start(ThreadId threadId)
    {
        if (threadId >= RPP_MAX_THREAD_COUNT)
        {
            RPP_UNREACHABLE(); // Invalid thread ID
        }

        Thread::ThreadData *pData = s_threadData[threadId];

        if (pData == nullptr)
        {
            RPP_UNREACHABLE(); // Thread not found
        }

        if (pData->isRunning)
        {
            RPP_UNREACHABLE(); // Thread already running
        }

        pData->isRunning = TRUE;
        std::thread *pThread = RPP_NEW(std::thread(pData->entry, pData->pData));
        pData->pHandle = pThread;
    }

    void Thread::Join(ThreadId threadId)
    {
        if (threadId >= RPP_MAX_THREAD_COUNT)
        {
            RPP_UNREACHABLE(); // Invalid thread ID
        }

        Thread::ThreadData *data = s_threadData[threadId];

        if (data == nullptr)
        {
            RPP_UNREACHABLE(); // Thread not found
        }

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