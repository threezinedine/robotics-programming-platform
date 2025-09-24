from .utils import GenerateFuncType, AssertGenerateResult


def test_bind_enum(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
/// @brief Color enumeration
enum RPP_PYTHON_BINDING Color {
    RED,
    GREEN, ///< Green color
    BLUE RPP_HIDE = 10,
};
""",
        "cpp_enum_binding.j2",
    )

    expected = """
py::enum_<Color>(m, "Color", py::arithmetic(), "Color enumeration")
    .value("RED", Color::RED , "" )
    .value("GREEN", Color::GREEN , "Green color" )
    .export_values();
"""

    AssertGenerateResult(expected, result)


def test_bind_struct(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
/// @brief Point structure
struct RPP_PYTHON_BINDING Point {
    int x; ///< X coordinate
    int y; ///< Y coordinate
};
""",
        "cpp_struct_binding.j2",
    )

    expected = """
py::class_<Point>(m, "Point", "Point structure")
    .def(py::init<>())
    .def_readwrite("x", &Point::x , "X coordinate" )
    .def_readwrite("y", &Point::y , "Y coordinate" );
"""

    AssertGenerateResult(expected, result)
