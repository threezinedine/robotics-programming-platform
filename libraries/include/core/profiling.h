#pragma once
#if defined(RPP_DEBUG)
#include "string.h"
#include "filesystem.h"

namespace rpp
{
    /**
     * A structure which holds profiling information for a specific code section.
     */
    struct ProfilingRecord // TODO: add thread id later
    {
        String file;                     ///< The name of the source file
        String func;                     ///< The name of the profiling record (usually function name or code section)
        u32 line;                        ///< The line number in the source file
        u32 start;                       ///< The starting time stamp (from the High Precision Timer)
        u32 end;                         ///< The ending time stamp (from the High Precision Timer)
        Array<ProfilingRecord> children; ///< Child profiling records for nested code sections
    };

    /**
     * A class which is used for tracking the performance, memory usage, and other profiling metrics of the whole system.
     */
    class Profiling
    {
    public:
        /**
         * Begins the tracking whereas the resources are stored the `ProfilingRecord` structures into a buffer.
         */
        static void StartRecord();

        /**
         * Free the resources and recorded data from the buffer.
         */
        static void EndRecord();

        /**
         * Each time this function is called, the recorded profiling data will be logged into the specified output file. The last file will be
         * closed and a new file will be created.
         *
         * @param outputFile The path to the output file where profiling data will be logged.
         * Default format: logs/log_YYYY_mm_dd_hh_mm_ss.log
         */
        static void StartLogging(const String &outputFile = "");

        /**
         * Stops logging profiling data to the current output file. From now, no more profiling data will be logged until `StartLogging` is called again.
         * The warning will be printed if no logging is currently active.
         */
        static void StopLogging();

    private:
        static ProfilingRecord *s_pCurrentRecords; ///< Pointer to the current profiling records buffer
        static FileHandle s_logFile;               ///< Handle to the log file where profiling data is written
        static char s_fileBuffer[1024];            ///< Buffer for file name (for logging purposes)
        static u32 s_indent;                       ///< Current indentation level for nested profiling records

    public:
        /**
         * @brief Constructs a Profiling object that starts recording profiling data for a specific code section.
         *
         * @param file The source file name where the profiling is initiated.
         * @param funcName The function name where the profiling is initiated.
         * @param line The line number in the source file where the profiling is initiated.
         */
        Profiling(const String &file, const String &funcName, u32 line);

        /**
         * @brief Destructs the Profiling object, ending the profiling for the code section.
         */
        ~Profiling();

    private:
        ProfilingRecord m_record; ///< The profiling record for this code section
    };
} // namespace rpp
#endif

#if defined(RPP_DEBUG)
#define RPP_PROFILE_SCOPE() rpp::Profiling profiling__LINE__(__FILE__, __FUNCTION__, __LINE__)
#else
#define RPP_PROFILE_SCOPE()
#endif