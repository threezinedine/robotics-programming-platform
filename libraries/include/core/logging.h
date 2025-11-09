#pragma once
#include "core/common.h"
#include "platforms/platforms.h"
#include "singleton_manager.h"
#include "containers/array.h"
#include "format.h"

namespace rpp
{
    /**
     * @brief All predefined types of the message log where the log message can be categorized based
     *      on the severity level.
     */
    enum class LogLevel
    {
        TRACE,   ///< Very detailed information, typically of interest only when diagnosing problems.
        DEBUG,   ///< Detailed information, typically of interest only when diagnosing problems.
        INFO,    ///< Confirmation that things are working as expected.
        WARNING, ///< An indication that something unexpected happened, or indicative of some problem in the near future (e.g. 'disk space low'). The software is still working as expected.
        ERROR,   ///< Due to a more serious problem, the software has not been able to perform some function.
        FATAL,   ///< A serious error, indicating that the program itself may be unable to continue running.
        COUNT RPP_HIDE,
    };

    /**
     * @brief an object which stores all needed information for a single log entry. This object must be handled
     *      by the handler of the logging system.
     */
    struct LogRecord
    {
        LogLevel level; ///< The severity level of the log message.
        String message; ///< The log message to be recorded.
        String file;    ///< The name of the source file where the log message originated.
        u32 line;       ///< The line number in the source file where the log message originated.
        u64 timestamp;  ///< The timestamp when the log message was created.
        u32 threadId;   ///< The ID of the thread that generated the log message.
    };

    /**
     * @brief Predefined types of log handlers. These types can be used to create standard handlers.
     *
     * @note CUSTOM type is reserved for user-defined handlers and is not associated with any predefined handler implementation.
     */
    enum class HandlerType : u8
    {
        CONSOLE = (0x01),      ///< Output log messages to the console (standard output).
        FILE = (0x01 << 1),    ///< Write log messages to a file.
        NETWORK = (0x01 << 2), ///< Send log messages over the network to a logging server.
        CUSTOM = (0x01 << 3),  ///< Use a custom handler defined by the user.
        COUNT RPP_HIDE,
    };

    /**
     * Interface for handling the log records. Users can implement this interface to define custom log handling behavior.
     */
    class Handler
    {
    public:
        virtual ~Handler();

        /**
         * @brief Handle a log record. This method is called by the logging system to process a log record.
         * @param record The log record to be handled.
         */
        void Handle(const LogRecord &record);

        inline LogLevel GetLevel() const { return m_level; }
        inline void SetLevel(LogLevel level) { m_level = level; }

    protected:
        /**
         * @brief Handle a log record. This method should be implemented by derived classes to define custom log handling behavior.
         * Implementations of this method should not throw exceptions.
         * @param record The log record to be handled.
         */
        virtual void HandleImpl(const LogRecord &record) = 0;

    private:
        LogLevel m_level = LogLevel::INFO; ///< The minimum log level that this handler will process.
    };

    /**
     * The logging system (singleton class) that provides logging functionalities.
     */
    class RPP_SINGLETON Logging
    {
        RPP_SINGLETON_DEFINE(Logging);

    public:
        /**
         * @brief Used for setting up the logging (useful for editor integration).
         *
         * @param type The type of handler to be used for logging, can be combined using bitwise OR.
         * @param level The minimum log level that the handler will process. Default is LogLevel::INFO.
         *
         * @example
         * ```cpp
         * Logging::GetInstance()->Setup(HandlerType::CONSOLE | HandlerType::FILE, LogLevel::DEBUG);
         * ```
         *
         * @note only callable once (the first call will be effective, subsequent calls will be ignored).
         */
        void Setup(u8 type, LogLevel level = LogLevel::INFO);

        /**
         * @brief Log a message with the specified log level, file name, and line number.
         * @param level The severity level of the log message.
         * @param message The log message to be recorded.
         * @param file The name of the source file where the log message originated.
         * @param line The line number in the source file where the log message originated.
         */
        void Log(LogLevel level, const String &message, const String &file, i32 line);

        /**
         * @brief Set up a new handler for processing log records. The logging system takes ownership of the handler.
         * @param handler A unique pointer to the handler to be added.
         */
        void SetupHandler(Scope<Handler> &&handler);

    private:
        Array<Scope<Handler>> m_handlers; ///< Array of handlers to process log records.
    };
} // namespace rpp

#if defined(RPP_LIBRARIES_TEST)
#if defined(RPP_DEBUG)
#define RPP_LOG_TRACE(msg, ...) rpp::Logging::GetInstance()->Log(rpp::LogLevel::TRACE, Format(msg, ##__VA_ARGS__), __FILE__, __LINE__)
#define RPP_LOG_DEBUG(msg, ...) rpp::Logging::GetInstance()->Log(rpp::LogLevel::DEBUG, Format(msg, ##__VA_ARGS__), __FILE__, __LINE__)
#else
#define RPP_LOG_TRACE(msg, ...)
#define RPP_LOG_DEBUG(msg, ...)
#endif

#define RPP_LOG_INFO(msg, ...) rpp::Logging::GetInstance()->Log(rpp::LogLevel::INFO, Format(msg, ##__VA_ARGS__), __FILE__, __LINE__)
#define RPP_LOG_WARNING(msg, ...) rpp::Logging::GetInstance()->Log(rpp::LogLevel::WARNING, Format(msg, ##__VA_ARGS__), __FILE__, __LINE__)
#define RPP_LOG_ERROR(msg, ...) rpp::Logging::GetInstance()->Log(rpp::LogLevel::ERROR, Format(msg, ##__VA_ARGS__), __FILE__, __LINE__)
#define RPP_LOG_FATAL(msg, ...) rpp::Logging::GetInstance()->Log(rpp::LogLevel::FATAL, Format(msg, ##__VA_ARGS__), __FILE__, __LINE__)
#else
#define RPP_LOG_TRACE(msg, ...)
#define RPP_LOG_DEBUG(msg, ...)
#define RPP_LOG_INFO(msg, ...)
#define RPP_LOG_WARNING(msg, ...)
#define RPP_LOG_ERROR(msg, ...)
#define RPP_LOG_FATAL(msg, ...)
#endif