#include "core/threading/threading.h"
#include "core/assertions.h"

#if defined(RPP_PLATFORM_WINDOWS)
#include <mutex>
#include <condition_variable>

namespace rpp
{
    Scope<Storage<Signal::SignalData>> Signal::s_signals = nullptr;

    struct SignalDataImpl
    {
        std::condition_variable handle; ///< The handle to the underlying signal object.
        b8 isSignaled;                  ///< Indicates whether the signal has been triggered.
        std::mutex mtx;                 ///< Mutex for synchronizing access to the signal state.
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

        b8 gotSignal = pImplData->handle.wait_for(
            std::unique_lock<std::mutex>(pImplData->mtx),
            std::chrono::milliseconds(timeout),
            [&pImplData]()
            {
                return pImplData->isSignaled;
            });

        return gotSignal;
    }

    void Signal::Notify(SignalId signal)
    {
        RPP_ASSERT(s_signals != nullptr);

        SignalData *data = s_signals->Get(signal);
        RPP_ASSERT(data != nullptr);

        SignalDataImpl *pImplData = static_cast<SignalDataImpl *>(data->pData);

        {
            std::lock_guard<std::mutex> lock(pImplData->mtx);
            pImplData->isSignaled = TRUE;
        }

        pImplData->handle.notify_all();
    }
} // namespace rpp

#endif