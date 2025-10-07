#include "core/threading/threading.h"
#include "core/assertions.h"

#if defined(RPP_PLATFORM_LINUX)
#include <pthread.h>
#include <unistd.h>
#include <limits>

namespace rpp
{
    Scope<Storage<Signal::SignalData>> Signal::s_signals = nullptr;

    struct SignalDataImpl
    {
        pthread_cond_t handle; ///< The handle to the underlying signal object.
        b8 isSignaled;         ///< Indicates whether the signal has been triggered.
        pthread_mutex_t mtx;   ///< Mutex for synchronizing access to the signal state.
    };

    void
    Signal::Initialize()
    {
        RPP_ASSERT(s_signals == nullptr);

        auto Deallocator = [](SignalData *data)
        {
            RPP_ASSERT(data != nullptr);
            RPP_ASSERT(data->pData != nullptr);

            SignalDataImpl *pImplData = static_cast<SignalDataImpl *>(data->pData);
            RPP_DELETE(pImplData);

            RPP_DELETE(data);
        };

        s_signals = CreateScope<Storage<SignalData>>(Deallocator);
    }

    void Signal::Shutdown()
    {
        RPP_ASSERT(s_signals != nullptr);
        s_signals.reset();
    }

    SignalId Signal::Create()
    {
        RPP_ASSERT(s_signals != nullptr);

        SignalId id = s_signals->Create();
        SignalData *data = s_signals->Get(id);
        RPP_ASSERT(data != nullptr);

        SignalDataImpl *pImplData = RPP_NEW(SignalDataImpl());
        pImplData->isSignaled = FALSE;
        pthread_cond_init(&pImplData->handle, nullptr);
        pthread_mutex_init(&pImplData->mtx, nullptr);

        data->pData = pImplData;

        return id;
    }

    void Signal::Destroy(SignalId signal)
    {
        RPP_ASSERT(s_signals != nullptr);
        s_signals->Free(signal);
    }

    b8 Signal::Wait(SignalId signal, u32 timeout)
    {
        RPP_ASSERT(s_signals != nullptr);

        if (timeout == INFINITE_WAIT)
        {
            timeout = std::numeric_limits<u32>::max();
        }

        SignalData *data = s_signals->Get(signal);
        RPP_ASSERT(data != nullptr);

        SignalDataImpl *pImplData = static_cast<SignalDataImpl *>(data->pData);

        pImplData->isSignaled = FALSE;

        b8 gotSignal = false;

        pthread_mutex_lock(&pImplData->mtx);
        while (!pImplData->isSignaled)
        {
            if (timeout == 0)
            {
                break;
            }

            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += timeout / 1000;
            ts.tv_nsec += (timeout % 1000) * 1000000;

            pthread_cond_timedwait(&pImplData->handle, &pImplData->mtx, &ts);
        }
        gotSignal = pImplData->isSignaled;
        pthread_mutex_unlock(&pImplData->mtx);

        return gotSignal;
    }

    void Signal::Notify(SignalId signal)
    {
        RPP_ASSERT(s_signals != nullptr);

        SignalData *data = s_signals->Get(signal);
        RPP_ASSERT(data != nullptr);

        SignalDataImpl *pImplData = static_cast<SignalDataImpl *>(data->pData);

        {
            pthread_mutex_lock(&pImplData->mtx);
            pImplData->isSignaled = TRUE;
        }

        pthread_cond_signal(&pImplData->handle);
        pthread_mutex_unlock(&pImplData->mtx);
    }
} // namespace rpp

#endif
