import pytest  # type: ignore
from .utils import GenerateFuncType, AssertGenerateResult


def test_bind_function_which_static(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class TestSystem
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
class TestSystem
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
class TestSystem
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
class TestSystem
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
"""

    AssertGenerateResult(expected, result)


def test_python_pyi_binding(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
class TestSystem
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
def Add(a: int, b: int) -> int:
    ...


def Log(message: str) -> None:
    ...


def Count() -> None:
    ...

"""

    AssertGenerateResult(expected, result)
