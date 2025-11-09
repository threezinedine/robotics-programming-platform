#if defined(RPP_DEBUG)
#include "core/profiling.h"
#include "platforms/platforms.h"
#include "core/assertions.h"
#include "platforms/timer.h"
#include <cstring>

namespace rpp
{
    ProfilingRecord *Profiling::s_pCurrentRecords = nullptr;
    FileHandle Profiling::s_logFile = INVALID_ID;
    char Profiling::s_fileBuffer[1024] = {0};

    void Profiling::StartRecord()
    {
        // Implementation for starting the profiling record
    }

    void Profiling::EndRecord()
    {
        // Implementation for ending the profiling record
    }

    void Profiling::StartLogging(const String &outputFile)
    {
        Datetime now = TimestampToDatetime(GetCurrentUNIXTimeStamp());
        String finalOutputFile = Format("{}_{}_{}_{}_{}_{}_{}.log", outputFile, now.year, now.month, now.day, now.hour, now.minute, now.second);

        if (s_logFile != INVALID_ID)
        {
            FileSystem::CloseFile(s_logFile);
        }

        RPP_ASSERT_MSG(s_logFile == INVALID_ID, "Profiling log file is already open.");

        // close if the file is already exists
        {
            b8 fileExists = FileSystem::IsPhysicalPathExists(finalOutputFile);
            if (fileExists)
            {
                FileSystem::DeletePhysicalFile(finalOutputFile);
            }
        }

        s_logFile = FileSystem::OpenFile(finalOutputFile, FILE_MODE_APPEND);
        memcpy(s_fileBuffer, finalOutputFile.CStr(), finalOutputFile.Length() + 1);

        RPP_ASSERT_MSG(s_logFile != INVALID_ID, "Failed to open profiling log file: {}", finalOutputFile);
        RPP_LOG_INFO("Profiling is logging started for file: {}", String(s_fileBuffer));
    }

    void Profiling::StopLogging()
    {
        RPP_ASSERT_MSG(s_logFile != INVALID_ID, "No profiling log file is currently open.");

        FileSystem::CloseFile(s_logFile);
        s_logFile = INVALID_ID;

        RPP_LOG_INFO("Profiling is logging stopped for file: {}", String(s_fileBuffer));
        memset(s_fileBuffer, 0, sizeof(s_fileBuffer));
    }

    Profiling::Profiling(const String &file, const String &funcName, u32 line)
    {
        m_record.file = file;
        m_record.func = funcName;
        m_record.line = line;
        m_record.start = GetCurrentUNIXTimeStamp();

        if (s_logFile != INVALID_ID)
        {
            Datetime now = TimestampToDatetime(m_record.start);
            String content = Format("{} ({}:{}) -- {}-{}-{} {}:{}:{}\n", m_record.func, m_record.file, m_record.line,
                                    now.year, now.month, now.day, now.hour, now.minute, now.second);

            FileSystem::Write(s_logFile, content);
        }
    }

    Profiling::~Profiling()
    {
        m_record.end = GetCurrentUNIXTimeStamp();

        if (s_logFile == INVALID_ID)
        {
            return;
        }

        Datetime now = TimestampToDatetime(m_record.end);
        String content = Format("{} ({}:{}) -- {}-{}-{} {}:{}:{}\n", m_record.func, m_record.file, m_record.line,
                                now.year, now.month, now.day, now.hour, now.minute, now.second);

        FileSystem::Write(s_logFile, content);
    }
} // namespace rpp
#endif