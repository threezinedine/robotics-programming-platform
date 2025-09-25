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
}
"""

    AssertGenerateResult(result, expected)
