#include "core/handlers/console_handlers.h"
#include "platforms/platforms.h"
#include "core/string.h"

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

        print("Logging\n", color);
        // print("[%s] %s:%d: %s\n", ToString(record.level).CStr(), record.file.CStr(), record.line, record.message.CStr());
    }

} // namespace rpp
