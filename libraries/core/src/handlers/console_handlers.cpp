#include "core/handlers/console_handlers.h"
#include "platforms/platforms.h"
#include "core/string.h"
#include "core/format.h"

namespace rpp
{
    ConsoleHandler::ConsoleHandler()
    {
    }

    ConsoleHandler::~ConsoleHandler()
    {
    }

    void ConsoleHandler::HandleImpl(const LogRecord &record)
    {
        ConsoleColor color = ConsoleColor::WHITE;

        switch (record.level)
        {
        case LogLevel::TRACE:
        case LogLevel::DEBUG:
        case LogLevel::INFO:
            color = ConsoleColor::WHITE;
            break;
        case LogLevel::WARNING:
            color = ConsoleColor::YELLOW;
            break;
        case LogLevel::ERROR:
        case LogLevel::FATAL:
            color = ConsoleColor::RED;
            break;
        };

        print(Format("[{}] {}:{}: {}\n", record.level, record.file, record.line, record.message).CStr(), color);
    }

} // namespace rpp
