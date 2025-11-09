#if defined(RPP_USE_TEST)
#include "modules/test_system/test_system_impl.h"
#include "modules/structures/structures.h"
#include "core/core.h"
#include <fstream>
#include <codecvt>
#include <exception>

#if defined(_DEBUG)
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

namespace rpp
{
    namespace
    {
#include "tmp/e2e_test_binding.cpp"
#include "tmp/e2e_json_writer_binding.cpp"
    } // namespace

    namespace
    {
        /**
         * Read the content of a python file from the given absolute path.
         *
         * @param filePath The absolute path to the python file. This the path must be physical path, not virtual path.
         */
        String ReadPhysicalFile(const String &filePath)
        {
            std::ifstream fileStream(filePath.CStr(), std::ios::in | std::ios::binary);
            if (fileStream)
            {
                try
                {
                    std::string content((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
                    return String(content.c_str());
                }
                catch (const std::exception &e)
                {
                    RPP_LOG_FATAL("Failed to read python file from path: {}, exception: {}", filePath, String(e.what()));
                    exit(-1);
                }
            }

            RPP_UNREACHABLE();
            return String();
        }

        void WritePhysicalFile(const String &filePath, const String &content)
        {
            std::ofstream fileStream(filePath.CStr(), std::ios::out | std::ios::binary);
            if (fileStream)
            {
                try
                {
                    fileStream.write(content.CStr(), content.Length());
                    fileStream.close();
                }
                catch (const std::exception &e)
                {
                    RPP_LOG_FATAL("Failed to write python file to path: {}, exception: {}", filePath, String(e.what()));
                    exit(-1);
                }
            }
            else
            {
                RPP_LOG_FATAL("Failed to open file stream for writing python file to path: {}", filePath);
                exit(-1);
            }
        }
    } // namespace anonymous

    RPP_SINGLETON_IMPLEMENT(TestSystem);

    TestSystem::TestSystem()
        : m_resultFilePath(""),
          m_setupScriptContent(""),
          m_updateScriptContent(""),
          m_shutdownScriptContent(""),
          m_runTestCaseName(""),
          m_testThreadSignal(INVALID_ID),
          m_mainThreadSignal(INVALID_ID),
          m_testThreadId(INVALID_ID),
          m_shouldApplicationClose(FALSE),
          m_isMainThreadWorking(FALSE)
    {
    }

    TestSystem::~TestSystem()
    {
    }

    void TestSystem::Initialize(const String &resultFilePath,
                                const String &setupFilePath,
                                const String &updateFilePath,
                                const String &shutdownFilePath,
                                const String &runTestCaseName)
    {
        RPP_PROFILE_SCOPE();

        m_resultFilePath = resultFilePath;
        m_runTestCaseName = runTestCaseName;

        m_shouldApplicationClose = FALSE;
        m_isMainThreadWorking = FALSE;

        // TODO: Loading scripts later
        if (setupFilePath.Length() > 0)
        {
            m_setupScriptContent = ReadPhysicalFile(setupFilePath);
        }
        else
        {
            m_setupScriptContent = "";
        }

        if (updateFilePath.Length() > 0)
        {
            m_updateScriptContent = ReadPhysicalFile(updateFilePath);
            RPP_LOG_INFO("Running test script {}", updateFilePath);
        }
        else
        {
            m_updateScriptContent = "";
        }

        if (shutdownFilePath.Length() > 0)
        {
            m_shutdownScriptContent = ReadPhysicalFile(shutdownFilePath);
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

        if (m_error != "")
        {
            throw std::runtime_error(m_error.CStr());
        }
    }

    void TestSystem::Wait(f32 milliseconds)
    {
        m_timer.Start();

        while (TRUE)
        {
            u32 elapsed = static_cast<u32>(m_timer.GetElapsedTimeInMilliseconds());
            if (elapsed >= static_cast<u32>(milliseconds))
            {
                break;
            }

            Yield();
        }

        Yield();
    }

    void TestSystem::Log(const String &message)
    {
        RPP_LOG_INFO("[TestSystem] {}", message);
    }

    void TestSystem::TestThreadFunction(void *arg)
    {
#include "tmp/e2e_test_append.cpp"
        Py_Initialize();
        RPP_UNUSED(arg);
        PyObject *globals = PyModule_GetDict(PyImport_AddModule("__main__"));

        if (globals == nullptr)
        {
            RPP_LOG_FATAL("Failed to get Python globals dictionary.");
            Py_Finalize();
            exit(-1);
        }

        // Run setup
        {
            PyRun_SimpleString(
                Format("import sys\nsys.path.append(\"{}/e2e\")",
                       String(STRINGIFY(RPP_PROJECT_DIR)))
                    .CStr());
            PyRun_SimpleString("from context import setup_context, run_context, teardown_context");
            PyRun_SimpleString(m_updateScriptContent.CStr());
            PyRun_SimpleString(Format("setup_context({})", m_runTestCaseName).CStr());
        }

        Signal::Notify(m_mainThreadSignal);
        Signal::Wait(m_testThreadSignal);

        String resultContent = ReadPhysicalFile(m_resultFilePath);
        Json resultsJson(resultContent);
        Json resultJson;
        resultJson.Set("name", m_runTestCaseName);

        try
        {
            RPP_LOG_INFO("Running test case '{}'", m_runTestCaseName);
#if 0
            int result = PyRun_SimpleString(Format("{}()", m_runTestCaseName).CStr());
#else
            int result = PyRun_SimpleString(Format("run_context({})", m_runTestCaseName).CStr());
#endif
            Yield();

            if (result != 0)
            {
                PyObject *type = nullptr, *value = nullptr, *traceback = nullptr;
                PyErr_Fetch(&type, &value, &traceback);
                PyErr_Print();

                if (type && value)
                {
                    RPP_LOG_DEBUG("Normalizing exception");
                    PyErr_NormalizeException(&type, &value, &traceback);
                }

                String errorMessage = "Unknown error";

                if (value != nullptr)
                {
                    PyObject *str_obj = PyObject_Str(value); // "boom! This is the error"
                    if (str_obj != nullptr)
                    {
                        const char *c_msg = PyUnicode_AsUTF8(str_obj);
                        if (c_msg != nullptr)
                        {
                            errorMessage = String(c_msg);
                        }
                        Py_DECREF(str_obj);
                    }
                }
                RPP_LOG_ERROR("Python error occurred during test execution: {}", errorMessage);

                // Save failure results
                resultJson.Set("status", false);
                resultJson.Set("error", String(errorMessage));

                Py_XDECREF(type);
                Py_XDECREF(value);
                Py_XDECREF(traceback);
            }
            else
            {
                resultJson.Set("status", true);
                resultJson.Set("error", String(""));
            }
        }
        catch (const std::exception &e)
        {
            RPP_LOG_ERROR("Exception occurred while executing python script: {}", String(e.what()));
            PyErr_Print();

            // Save failure results
            {
                resultJson.Set("status", false);
                resultJson.Set("error", String(e.what()));
            }
        }

        if (m_error != "")
        {
            resultJson.Set("status", false);
            resultJson.Set("error", m_error);
        }

        resultsJson.Append(resultJson);
        WritePhysicalFile(m_resultFilePath, resultsJson.ToString());

        m_shouldApplicationClose = TRUE;
        Signal::Notify(m_mainThreadSignal); // exit the test thread

        Py_Finalize();
    }

    void TestSystem::Setup()
    {
        Signal::Wait(m_mainThreadSignal);
    }

    void TestSystem::Update(f32 deltaTime)
    {
        RPP_UNUSED(deltaTime);
        if (m_isMainThreadWorking)
        {
            return;
        }

        Signal::Notify(m_testThreadSignal);
        Signal::Wait(m_mainThreadSignal);
    }

    b8 TestSystem::ShouldApplicationClose()
    {
        return m_shouldApplicationClose;
    };
} // namespace rpp
#endif