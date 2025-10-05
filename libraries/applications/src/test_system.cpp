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

    RPP_SINGLETON_IMPLEMENT(TestSystem);

    TestSystem::TestSystem()
        : m_resultFilePath(""),
          m_setupScriptContent(""),
          m_updateScriptContent(""),
          m_shutdownScriptContent(""),
          m_testThreadSignal(INVALID_ID),
          m_mainThreadSignal(INVALID_ID),
          m_testThreadId(INVALID_ID),
          m_shouldApplicationClose(FALSE)
    {
    }

    TestSystem::~TestSystem()
    {
    }

    void TestSystem::Initialize(const String &resultFilePath,
                                const String &setupFilePath,
                                const String &updateFilePath,
                                const String &shutdownFilePath)
    {
        m_resultFilePath = resultFilePath;
        m_shouldApplicationClose = FALSE;

        // TODO: Loading scripts later
        if (setupFilePath.Length() > 0)
        {
            m_setupScriptContent = ReadPythonFile(setupFilePath);
        }
        else
        {
            m_setupScriptContent = "";
        }

        if (updateFilePath.Length() > 0)
        {
            m_updateScriptContent = ReadPythonFile(updateFilePath);
        }
        else
        {
            m_updateScriptContent = "";
        }

        if (shutdownFilePath.Length() > 0)
        {
            m_shutdownScriptContent = ReadPythonFile(shutdownFilePath);
        }
        else
        {
            m_shutdownScriptContent = "";
        }

        RPP_ASSERT(m_testThreadSignal == INVALID_ID && m_mainThreadSignal == INVALID_ID);
        RPP_ASSERT(m_testThreadId == INVALID_ID);

        m_testThreadSignal = Signal::Create();
        m_mainThreadSignal = Signal::Create();
        m_testThreadId = Thread::Create(TestThreadStaticFunction);

        Py_SetPythonHome(ConvertCharToWChar(STRINGIFY(RPP_PYTHON_HOME)));

        // TODO: Run setup script if provided

        Thread::Start(m_testThreadId);
    }

    void TestSystem::TestThreadStaticFunction(void *pParam)
    {
        TestSystem::GetInstance()->TestThreadFunction(pParam);
    }

    void TestSystem::Shutdown()
    {
    }

    void TestSystem::Yield()
    {
        Signal::Notify(m_mainThreadSignal);
        Signal::Wait(m_testThreadSignal);
    }

    void TestSystem::Wait(f32 milliseconds)
    {
        m_timer.Start();
        RPP_LOG_DEBUG("TestSystem::Wait for {} milliseconds", milliseconds);
        return;

        while (TRUE)
        {
            u32 elapsed = static_cast<u32>(m_timer.GetElapsedTimeInMilliseconds());
            if (elapsed >= static_cast<u32>(milliseconds))
            {
                break;
            }

            Yield();
        }
    }

    void TestSystem::TestThreadFunction(void *arg)
    {
        RPP_UNUSED(arg);
        Signal::Wait(m_testThreadSignal);

        Array<String> lineOfCodes;
        m_updateScriptContent.Split(lineOfCodes, "\n");

        u32 numberOfLines = lineOfCodes.Size();
        pybind11::scoped_interpreter guard{};

        for (u32 lineIndex = 0; lineIndex < numberOfLines; lineIndex++)
        {
            const String &line = lineOfCodes[lineIndex];

            try
            {
                pybind11::exec(line.CStr());

                if (!m_commandStack.Empty())
                {
                    Command command = m_commandStack.Pop();
                    switch (command)
                    {
                    case Command::COMMAND_WAIT:
                        Wait(1000); // wait for 100 milliseconds
                        break;
                    default:
                        RPP_UNREACHABLE();
                        break;
                    }
                }
            }
            catch (const std::exception &e)
            {
                RPP_LOG_ERROR("Exception occurred while executing python line {}: {}\n\t", lineIndex + 1, String(e.what()), line);
                break;
            }

            m_shouldApplicationClose = TRUE;
            Signal::Notify(m_mainThreadSignal); // exit the test thread
        }

        m_shouldApplicationClose = TRUE;
        Signal::Notify(m_mainThreadSignal); // exit the test thread
    }

    void TestSystem::Update(f32 deltaTime)
    {
        RPP_UNUSED(deltaTime);
        Signal::Notify(m_testThreadSignal);
        Signal::Wait(m_mainThreadSignal);
    }

    b8 TestSystem::ShouldApplicationClose()
    {
        return m_shouldApplicationClose;
    };
} // namespace rpp