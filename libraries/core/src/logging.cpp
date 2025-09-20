#include "core/logging.h"
#include <cstdio>

namespace rpp
{
    Handler::~Handler()
    {
    }

    void Handler::Handle(const LogRecord &record)
    {
        if (record.level < m_level)
        {
            return;
        }

        HandleImpl(record);
    }

    RPP_SINGLETON_IMPLEMENT(Logging);

    Logging::Logging()
    {
    }

    Logging::~Logging()
    {
    }

    void Logging::Log(LogLevel level, const String &message, const String &file, i32 line)
    {
        LogRecord record;
        record.level = level;
        record.message = message;
        record.file = file;
        record.line = static_cast<u32>(line);
        record.timestamp = 0;
        record.threadId = 0;

        u32 handlerCount = m_handlers.Size();
        for (u32 i = 0; i < handlerCount; i++)
        {
            m_handlers[i]->Handle(record);
        }
    }

    void Logging::SetupHandler(const Scope<Handler> &handler)
    {
        m_handlers.Push(std::move(handler));
    }
} // namespace rpp
