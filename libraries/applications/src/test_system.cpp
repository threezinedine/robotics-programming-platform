#include "applications/test_system/test_system_impl.h"
#include "applications/graphic_session/graphic_session.h"
#include "core/core.h"
#include <fstream>
#include <codecvt>

#include "pybind11/pybind11.h"
#include "pybind11/embed.h"

namespace rpp
{
    namespace
    {
        const wchar_t *ConvertCharToWChar(const char *str)
        {
            static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            static std::wstring wideStr = converter.from_bytes(str);
            return wideStr.c_str();
        }
    } // namespace

    namespace
    {
        /**
         * Read the content of a python file from the given absolute path.
         *
         * @param filePath The absolute path to the python file. This the path must be physical path, not virtual path.
         */
        String ReadPythonFile(const String &filePath)
        {
            std::ifstream fileStream(filePath.CStr(), std::ios::in | std::ios::binary);
            if (fileStream)
            {
                std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
                return String(content.c_str());
            }

            RPP_UNREACHABLE();
            return String();
        }
    } // namespace anonymous

    String TestSystem::s_setupScriptContent = "";
    String TestSystem::s_updateScriptContent = "";
    String TestSystem::s_shutdownScriptContent = "";
    String TestSystem::s_resultFilePath = "";

    SignalId TestSystem::s_testThreadSignal = INVALID_ID;
    SignalId TestSystem::s_mainThreadSignal = INVALID_ID;
    ThreadId TestSystem::s_testThreadId = INVALID_ID;
    b8 TestSystem::s_shouldApplicationClose = FALSE;

    void TestSystem::Initialize(const String &resultFilePath,
                                const String &setupFilePath,
                                const String &updateFilePath,
                                const String &shutdownFilePath)
    {
        s_resultFilePath = resultFilePath;
        s_shouldApplicationClose = FALSE;

        // TODO: Loading scripts later
        if (setupFilePath.Length() > 0)
        {
            s_setupScriptContent = ReadPythonFile(setupFilePath);
        }
        else
        {
            s_setupScriptContent = "";
        }

        if (updateFilePath.Length() > 0)
        {
            s_updateScriptContent = ReadPythonFile(updateFilePath);
        }
        else
        {
            s_updateScriptContent = "";
        }

        if (shutdownFilePath.Length() > 0)
        {
            s_shutdownScriptContent = ReadPythonFile(shutdownFilePath);
        }
        else
        {
            s_shutdownScriptContent = "";
        }

        RPP_ASSERT(s_testThreadSignal == INVALID_ID && s_mainThreadSignal == INVALID_ID);
        RPP_ASSERT(s_testThreadId == INVALID_ID);

        s_testThreadSignal = Signal::Create();
        s_mainThreadSignal = Signal::Create();
        s_testThreadId = Thread::Create(TestThreadFunction);

        Py_SetPythonHome(ConvertCharToWChar(STRINGIFY(RPP_PYTHON_HOME)));

        // TODO: Run setup script if provided

        Thread::Start(s_testThreadId);
    }

    void TestSystem::Shutdown()
    {
        // TODO: Run shutdown script if provided

        Signal::Destroy(s_testThreadSignal);
        Signal::Destroy(s_mainThreadSignal);
        Thread::Destroy(s_testThreadId);

        s_resultFilePath.~String();
        s_setupScriptContent.~String();
        s_updateScriptContent.~String();
        s_shutdownScriptContent.~String();
    }

    void TestSystem::Yield()
    {
        Signal::Notify(s_mainThreadSignal);
        Signal::Wait(s_testThreadSignal);
    }

    void TestSystem::TestThreadFunction(void *arg)
    {
        RPP_UNUSED(arg);
        Signal::Wait(s_testThreadSignal);

        Array<String> lineOfCodes;
        s_updateScriptContent.Split(lineOfCodes, "\n");

        u32 numberOfLines = lineOfCodes.Size();
        pybind11::scoped_interpreter guard{};

        for (u32 lineIndex = 0; lineIndex < numberOfLines; ++lineIndex)
        {
            const String &line = lineOfCodes[lineIndex];
            try
            {
                pybind11::exec(line.CStr());
            }
            catch (const std::exception &e)
            {
                RPP_LOG_ERROR("Exception occurred while executing python line {}: {}\n\t", lineIndex + 1, String(e.what()), line);
                break;
            }
        }

        s_shouldApplicationClose = TRUE;
        Signal::Notify(s_mainThreadSignal); // exit the test thread
    }

    void TestSystem::Update(f32 deltaTime)
    {
        RPP_UNUSED(deltaTime);
        Signal::Notify(s_testThreadSignal);
        Signal::Wait(s_mainThreadSignal);
    }

    b8 TestSystem::ShouldApplicationClose()
    {
        return s_shouldApplicationClose;
    };
} // namespace rpp