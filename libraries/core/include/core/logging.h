#pragma once
#include "platforms/platforms.h"
#include "singleton_manager.h"
#include "array.h"

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
        FATAL,   /////< A serious error, indicating that the program itself may be unable to continue running.
        COUNT,
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

    String ToString(LogLevel level);

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
    class Logging
    {
        RPP_SINGLETON_DEFINE(Logging);

    public:
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
        void SetupHandler(const Scope<Handler> &handler);

    private:
        Array<Scope<Handler>> m_handlers; ///< Array of handlers to process log records.
    };
} // namespace rpp

#define RPP_LOG_TRACE(msg) rpp::Logging::GetInstance()->Log(rpp::LogLevel::TRACE, msg, __FILE__, __LINE__)
#define RPP_LOG_DEBUG(msg) rpp::Logging::GetInstance()->Log(rpp::LogLevel::DEBUG, msg, __FILE__, __LINE__)
#define RPP_LOG_INFO(msg) rpp::Logging::GetInstance()->Log(rpp::LogLevel::INFO, msg, __FILE__, __LINE__)
#define RPP_LOG_WARNING(msg) rpp::Logging::GetInstance()->Log(rpp::LogLevel::WARNING, msg, __FILE__, __LINE__)
#define RPP_LOG_ERROR(msg) rpp::Logging::GetInstance()->Log(rpp::LogLevel::ERROR, msg, __FILE__, __LINE__)
#define RPP_LOG_FATAL(msg) rpp::Logging::GetInstance()->Log(rpp::LogLevel::FATAL, msg, __FILE__, __LINE__)
