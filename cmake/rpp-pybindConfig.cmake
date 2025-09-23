if (rpp-pybind_FOUND)
    return()
endif()

find_package(Python REQUIRED COMPONENTS Interpreter Development)

set(rpp-pybind_DIRS ${RPP_EXTERNALS_DIR}/pybind)
add_subdirectory(${rpp-pybind_DIRS} ${CMAKE_BINARY_DIR}/externals/pybind)

set(rpp-pybind_FOUND TRUE)