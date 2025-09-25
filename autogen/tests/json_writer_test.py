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

    result["major"] = value.major;
    result["minor"] = value.minor;
    result["patch"] = value.patch;
    return result.ToString();
}

template<>
bool FromString<Version>(const String& str, Version &value)
{
    Json json(str);

    value.major = json.Get<char>("major", value.major);
    value.minor = json.Get<char>("minor", value.minor);
    value.patch = json.Get<char>("patch", value.patch);

    return true;
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

    result["count"] = value.count;
    return result.ToString();
}

template<>
bool FromString<Test>(const String& str, Test &value)
{
    Json json(str);
    value.count = json.Get<int>("count", value.count);
    return true;
}

template<>
const String ToString<Container>(const Container &value)
{
    Json result;

    result["id"] = value.id;
    result["test"] = Json(ToString(value.test));
    return result.ToString();
}

template<>
bool FromString<Container>(const String& str, Container &value)
{
    Json json(str);
    value.id = json.Get<int>("id", value.id);
    FromString(json.Get<String>("test", "{}"), value.test);
    return true;
}
"""

    AssertGenerateResult(result, expected)
