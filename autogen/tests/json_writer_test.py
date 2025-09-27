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
    Version value;

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
    Test value;

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
    Container value;

    value.id = json.Get<int>(String("id"), value.id);
    FromString(json.Get<String>(String("test"), String("{}")), value.test);
    return value;
}
"""

    AssertGenerateResult(result, expected)


@pytest.mark.skip(reason="Array not supported yet")
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
    result.Set(String("values"), Json(ToString(value.values)));
    return result.ToString();
}

template<>
Item FromString<Item>(const String& str)
{
    Json json(str);
    Item value;

    value.id = json.Get<int>(String("id"), value.id);
    FromString(json.Get<String>(String("values"), String("{}")), value.values);
    return value;
}
"""

    AssertGenerateResult(result, expected)
