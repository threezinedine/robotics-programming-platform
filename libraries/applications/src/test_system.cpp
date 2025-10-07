#include "applications/test_system/test_system_impl.h"
#include "applications/graphic_session/graphic_session.h"
#include "core/core.h"
#include <fstream>
#include <codecvt>

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
        static PyObject *Wait(PyObject *self, PyObject *args)
        {
            float milliseconds;
            if (!PyArg_ParseTuple(args, "f", &milliseconds))
            {
                return nullptr;
            }

            TestSystem::GetInstance()->Wait(milliseconds);

            Py_RETURN_NONE;
        }

        static PyMethodDef TestSystemMethods[] = {
            {"Wait", Wait, METH_VARARGS, "Wait for the specified milliseconds."},
            {nullptr, nullptr, 0, nullptr} // Sentinel
        };

        static struct PyModuleDef TestSystemModule = {
            PyModuleDef_HEAD_INIT,
            "TestSystem", // name of module
            nullptr,      // module documentation, may be NULL
            -1,           // size of per-interpreter state of the module,
                          // or -1 if the module keeps state in global variables.
            TestSystemMethods};

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

    void TestSystem::TestThreadFunction(void *arg)
    {
        PyImport_AppendInittab("TestSystem", [](void) -> PyObject *
                               { return PyModule_Create(&TestSystemModule); });

        Py_Initialize();
        RPP_UNUSED(arg);
        Signal::Wait(m_testThreadSignal);

        Array<String> lineOfCodes;
        m_updateScriptContent.Split(lineOfCodes, "\n");

        u32 numberOfLines = lineOfCodes.Size();

        for (u32 lineIndex = 0; lineIndex < numberOfLines; lineIndex++)
        {
            const String &line = lineOfCodes[lineIndex];

            try
            {
                PyRun_SimpleString(line.CStr());
            }
            catch (const std::exception &e)
            {
                RPP_LOG_ERROR("Exception occurred while executing python line {}: {}\n\t", lineIndex + 1, String(e.what()), line);
                break;
            }

            m_shouldApplicationClose = FALSE;
            Signal::Notify(m_mainThreadSignal); // exit the test thread
        }

        m_shouldApplicationClose = TRUE;
        Signal::Notify(m_mainThreadSignal); // exit the test thread

        Py_Finalize();
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