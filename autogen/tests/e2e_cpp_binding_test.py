import pytest  # type: ignore
from .utils import GenerateFuncType, AssertGenerateResult


def test_bind_function_which_static(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class RPP_E2E_BINDING RPP_SINGLETON TestSystem
{
public:
    static int Add(int a, int b) RPP_E2E_BINDING;
};
""",
        "e2e_test_cpp_function_binding.j2",
        [],
    )

    expected = """
"""

    AssertGenerateResult(expected, result)


def test_binding_function_which_not_test_system(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class AnotherClass
{
public: 
    int Add(int a, int b) RPP_E2E_BINDING;
};
        """,
        "e2e_test_cpp_function_binding.j2",
        [],
    )

    expected = """
    """
    AssertGenerateResult(expected, result)


def test_bind_function(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class RPP_E2E_BINDING RPP_SINGLETON TestSystem
{
public:
    int Add(int a, int b) RPP_E2E_BINDING;

protected:
    int Subtract(int a, int b) RPP_E2E_BINDING; // Should not be bound as it's protected
};
""",
        "e2e_test_cpp_function_binding.j2",
        [],
    )

    expected = """
static PyObject* Add(PyObject* self, PyObject* args)
{
    int a;
    int b;
    if (!PyArg_ParseTuple(args, "ii", &a, &b))
    {
        return nullptr;
    }

    auto result = ::rpp::TestSystem::GetInstance()->Add(a, b);

    return Py_BuildValue("i", result);
}
"""

    AssertGenerateResult(expected, result)


def test_bind_function_with_no_return_type(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class RPP_E2E_BINDING RPP_SINGLETON TestSystem
{
public:
    void Log(const std::string& message) RPP_E2E_BINDING;
};
""",
        "e2e_test_cpp_function_binding.j2",
        ["string"],
    )

    expected = """
static PyObject* Log(PyObject* self, PyObject* args)
{
    const char* message;
    if (!PyArg_ParseTuple(args, "s", &message))
    {
        return nullptr;
    }

    ::rpp::TestSystem::GetInstance()->Log(message);
    Py_RETURN_NONE;
}
"""

    AssertGenerateResult(expected, result)


def test_methods_define(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class RPP_E2E_BINDING RPP_SINGLETON TestSystem
{
public:
    int Add(int a, int b) RPP_E2E_BINDING;
    void Log(const char* message) RPP_E2E_BINDING;

    void Count() RPP_E2E_BINDING;
};
""",
        "e2e_module_define.j2",
        [],
    )

    expected = """
static PyMethodDef TestSystemMethods[] = {
    {"Add", Add, METH_VARARGS, ""},
    {"Log", Log, METH_VARARGS, ""},
    {"Count", Count, METH_NOARGS, ""},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef TestSystemModule = {
    PyModuleDef_HEAD_INIT,
    "TestSystem",
    nullptr,
    -1,
    TestSystemMethods
};

"""

    AssertGenerateResult(expected, result)


def test_python_pyi_binding(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class RPP_E2E_BINDING RPP_SINGLETON TestSystem
{
public:
    int Add(int a, int b) RPP_E2E_BINDING;
    void Log(const std::string &message) RPP_E2E_BINDING;

    void Count() RPP_E2E_BINDING;
};
""",
        "e2e_python_function_binding.j2",
        ["string"],
    )

    expected = """
class TestSystem:
    @staticmethod
    def Add(a: int, b: int) -> int:
        ...

    @staticmethod
    def Log(message: str) -> None:
        ...

    @staticmethod
    def Count() -> None:
        ...

"""

    AssertGenerateResult(expected, result)


def test_parse_the_filesystem_utils(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
typedef unsigned int u32;

class RPP_E2E_BINDING FileSystem
{
public:
    static u32 OpenFile(const std::string& path) RPP_E2E_BINDING;
};
""",
        "e2e_test_cpp_function_binding.j2",
        ["string"],
    )

    expected = """
static PyObject* OpenFile(PyObject* self, PyObject* args)
{
    const char* path;
    if (!PyArg_ParseTuple(args, "s", &path))
    {
        return nullptr;
    }

    auto result = ::rpp::FileSystem::OpenFile(path);

    return Py_BuildValue("i", result);
}
"""

    AssertGenerateResult(expected, result)


def test_module_define(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class RPP_E2E_BINDING RPP_SINGLETON TestSystem
{
public:
    int Add(int a, int b) RPP_E2E_BINDING;
    void Log(const char* message) RPP_E2E_BINDING;
};

class RPP_E2E_BINDING FileSystem
{
public:
    static unsigned int OpenFile(const std::string& path) RPP_E2E_BINDING;
};
""",
        "e2e_module_define.j2",
        [],
    )

    expected = """
static PyMethodDef TestSystemMethods[] = {
    {"Add", Add, METH_VARARGS, ""},
    {"Log", Log, METH_VARARGS, ""},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef TestSystemModule = {
    PyModuleDef_HEAD_INIT,
    "TestSystem",
    nullptr,
    -1,
    TestSystemMethods
};

static PyMethodDef FileSystemMethods[] = {
    {"OpenFile", OpenFile, METH_VARARGS, ""},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef FileSystemModule = {
    PyModuleDef_HEAD_INIT,
    "FileSystem",
    nullptr,
    -1,
    FileSystemMethods
};
"""

    AssertGenerateResult(expected, result)


def test_module_register(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class RPP_E2E_BINDING RPP_SINGLETON TestSystem
{
public:
    int Add(int a, int b) RPP_E2E_BINDING;
    void Log(const char* message) RPP_E2E_BINDING;
};

class RPP_E2E_BINDING FileSystem
{
public:
    static unsigned int OpenFile(const std::string& path) RPP_E2E_BINDING;
};
""",
        "e2e_module_register.j2",
        [],
    )

    expected = """
PyImport_AppendInittab("TestSystem", [](void) -> PyObject* {
    return PyModule_Create(&TestSystemModule);
});

PyImport_AppendInittab("FileSystem", [](void) -> PyObject* {
    return PyModule_Create(&FileSystemModule);
});

"""

    AssertGenerateResult(expected, result)


def test_python_pyi_module_bind_enum(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
enum class RPP_E2E_BINDING Color
{
    Red = 0,
    Green = 1,
    Blue = 2
};
""",
        "e2e_python_enum_binding.j2",
        [],
    )

    expected = """
class Color(Enum):
    Red = 0
    Green = 1
    Blue = 2

"""

    AssertGenerateResult(expected, result)


def test_enum_define_binding(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
enum class RPP_E2E_BINDING Color
{
    Red = 0,
    Green = 1,
    Blue = 2
};
""",
        "e2e_module_enum_create.j2",
        [],
    )

    expected = """
PyRun_SimpleString(R"(
from enum import *

class Color(Enum):
    Red = 0
    Green = 1
    Blue = 2
)");
"""

    AssertGenerateResult(expected, result)
