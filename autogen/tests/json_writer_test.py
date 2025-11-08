import pytest  # type: ignore
from .utils import GenerateFuncType, AssertGenerateResult


def test_simple_object(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
struct RPP_JSON Version
{
    char major RPP_JSON_KEY("major");
    char minor RPP_JSON_KEY("minor");
    char patch RPP_JSON_KEY("patch");
    int nonMapped;
};
""",
        "json_writer_binding.j2",
        [],
    )

    expected = """
template<>
const String ToString<Version>(const Version &value)
{
    Json result;

    result.Set(String("major"), value.major);
    result.Set(String("minor"), value.minor);
    result.Set(String("patch"), value.patch);
    return result.ToString();
}

template<>
Version FromString<Version>(const String& str)
{
    Json json(str);
    Version value = {};

    value.major = json.Get<char>(String("major"), value.major);
    value.minor = json.Get<char>(String("minor"), value.minor);
    value.patch = json.Get<char>(String("patch"), value.patch);

    return value;
}
"""

    AssertGenerateResult(result, expected)


def test_nested_object(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
struct RPP_JSON Test
{
    int count RPP_JSON_KEY("count");
};

struct RPP_JSON Container
{
    int id RPP_JSON_KEY("id");
    Test test RPP_JSON_KEY("test");
};
""",
        "json_writer_binding.j2",
        [],
    )

    expected = """
template<>
const String ToString<Test>(const Test &value)
{
    Json result;

    result.Set(String("count"), value.count);
    return result.ToString();
}

template<>
Test FromString<Test>(const String& str)
{
    Json json(str);
    Test value = {};

    value.count = json.Get<int>(String("count"), value.count);
    return value;
}

template<>
const String ToString<Container>(const Container &value)
{
    Json result;

    result.Set(String("id"), value.id);
    result.Set(String("test"), Json(ToString(value.test)));
    return result.ToString();
}

template<>
Container FromString<Container>(const String& str)
{
    Json json(str);
    Container value = {};

    value.id = json.Get<int>(String("id"), value.id);
    FromString(json.Get<String>(String("test"), String("{}")), value.test);
    return value;
}
"""

    AssertGenerateResult(result, expected)


def test_array_attribute(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
struct RPP_JSON Item
{
    int id RPP_JSON_KEY("id");
    Array<int> values RPP_JSON_KEY("values");
};
""",
        "json_writer_binding.j2",
        ["string"],
    )

    expected = """
template<>
const String ToString<Item>(const Item &value)
{
    Json result;

    result.Set(String("id"), value.id);

    {
        u32 fieldsCount = value.values.Size();
        Json fieldJson = Json("[]");
        for (u32 i = 0; i < fieldsCount; i++)
        {
            fieldJson.Append<int>(value.values[i]);
        }   
        result.Set(String("values"), fieldJson);
    }

    return result.ToString();
}

template<>
Item FromString<Item>(const String& str)
{
    Json json(str);
    Item value = {};

    value.id = json.Get<int>(String("id"), value.id);

    {
        Json arrayField = json.Get<Json>(String("values"));
        RPP_ASSERT_MSG(arrayField.IsArray(), "The field 'values' is not an array.");
        u32 arraySize = arrayField.Size();
        for (u32 i = 0; i < arraySize; i++)
        {
            value.values.Push(arrayField.Get<int>(i));
        }
    }

    return value;
}
"""

    AssertGenerateResult(result, expected)


def test_e2e_json_writer_binding(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
struct RPP_JSON Address
{
    int street RPP_JSON_KEY("street");
    float city RPP_JSON_KEY("city");
};
""",
        "e2e_json_writer_binding.j2",
        ["string"],
    )

    expected = """
#if 0
struct AddressObject
{
    PyObject_HEAD
    Address data;
};

static PyTypeObject AddressType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Address",
    .tp_basicsize = sizeof(AddressObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_dealloc = (destructor)PyObject_Del,
};

static PyObject* Create_Address(PyObject* self, PyObject* args)
{
    RPP_ASSERT(PyType_Ready(&AddressType) >= 0);

    RPP_LOG_DEBUG("Here");
    AddressObject* obj = PyObject_New(AddressObject, &AddressType);

    RPP_LOG_DEBUG("Here");
    if (!obj)
    {
        return nullptr;
    }

    // Initialize fields to default values
    obj->data = Address();

    return (PyObject*)obj;
}

static PyObject* ToString_Address(PyObject* self, PyObject* args)
{
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O!", &AddressType, &obj))
    {
        return nullptr;
    }

    if (!PyObject_TypeCheck(obj, &AddressType))
    {
        PyErr_SetString(PyExc_TypeError, "Invalid object type passed to ToString_Address");
        return nullptr;
    }

    AddressObject* structObj = (AddressObject*)obj;
    const char* jsonStr = ToString<Address>(structObj->data).CStr();
    return Py_BuildValue("s", jsonStr);
}

static PyObject* FromString_Address(PyObject* self, PyObject* args)
{
    try
    {
        const char* str;
        if (!PyArg_ParseTuple(args, "s", &str))
        {
            return nullptr;
        }

        AddressObject* obj = PyObject_New(AddressObject, &AddressType);
        if (!obj)
        {
            return nullptr;
        }

        obj->data = FromString<Address>(String(str));
        return (PyObject*)obj;
    }
    catch (const std::exception& e)
    {
        PyErr_SetString(PyExc_Exception, e.what());
        return nullptr;
    }
}
#endif
"""

    AssertGenerateResult(result, expected)
