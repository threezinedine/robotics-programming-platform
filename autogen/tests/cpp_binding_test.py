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
        [],
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
        [],
    )

    expected = """
py::class_<Point>(m, "Point", "Point structure")
    .def(py::init<>())
    .def_readwrite("x", &Point::x , "X coordinate" )
    .def_readwrite("y", &Point::y , "Y coordinate" );
"""

    AssertGenerateResult(expected, result)


def test_bind_class(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
/// @brief The logging system (singleton class) that provides logging functionalities.
class RPP_PYTHON_BINDING Timer {
public:
    /// @brief Start the timer
    void Start() RPP_PYTHON_BINDING; 

    double Elapsed() RPP_PYTHON_BINDING;
};
""",
        "cpp_class_binding.j2",
        [],
    )

    expected = """
py::class_<Timer>(m, "Timer", "The logging system (singleton class) that provides logging functionalities.")
    .def(py::init<>())
    .def("Start", &Timer::Start , "Start the timer" )
    .def("Elapsed", &Timer::Elapsed , "" );
"""

    AssertGenerateResult(expected, result)


def test_bind_singleton_class(generateFunc: GenerateFuncType) -> None:
    result = generateFunc(
        """
/// @brief The logging system (singleton class) that provides logging functionalities.
class RPP_PYTHON_BINDING RPP_SINGLETON Logging {
public:
    /// @brief Log a message with the specified log level, file name, and line number.
    void Log(int level, const std::string &message, const std::string &file, int line) RPP_PYTHON_BINDING;
};
""",
        "cpp_class_binding.j2",
        ["string"],
    )

    expected = """
py::class_<Logging>(m, "Logging", "The logging system (singleton class) that provides logging functionalities.")
    .def_static("Log", [](int level, const std::string &message, const std::string &file, int line) { 
        return Logging::GetInstance()->Log(level, message.c_str(), file.c_str(), line);
    } , "Log a message with the specified log level, file name, and line number." );
"""

    AssertGenerateResult(expected, result)
