import pytest  # type: ignore
from type_map.type_map import TypeMap


def test_raw_type_map_parsing() -> None:
    typeMap = TypeMap()

    assert typeMap.Convert("int") == "int"
    assert typeMap.Convert("float") == "float"


def test_extended_type_map_parsing() -> None:
    typeMap = TypeMap()
    typeMap.AddMapping(r"^std::string$", "str")

    assert typeMap.Convert("int") == "int"
    assert typeMap.Convert("float") == "float"
    assert typeMap.Convert("std::string") == "str"
