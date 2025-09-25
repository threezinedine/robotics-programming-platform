#include "core/logging.h"
#include <cstdio>
#include "core/handlers/handlers.h"

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

    void Logging::Setup(u8 type, LogLevel level)
    {
        static bool isSetup = false;
        if (isSetup)
        {
            return;
        }
        isSetup = true;

        if (type & static_cast<u8>(HandlerType::CONSOLE))
        {
            Scope<Handler> consoleHandler = CreateScope<ConsoleHandler>();
            consoleHandler->SetLevel(level);
            SetupHandler(std::move(consoleHandler));
        }

        u32 handlerCount = m_handlers.Size();
        for (u32 i = 0; i < handlerCount; i++)
        {
            m_handlers[i]->SetLevel(level);
        }
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

    void Logging::SetupHandler(Scope<Handler> &&handler)
    {
        m_handlers.Push(std::move(handler));
    }

    template <>
    const String ToString<LogLevel>(const LogLevel &level)
    {
        switch (level)
        {
        case LogLevel::TRACE:
            return "TRACE";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }

} // namespace rpp
