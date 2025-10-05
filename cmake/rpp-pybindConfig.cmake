if (TARGET rpp-pybind)
    return()
endif()

find_package(Python REQUIRED COMPONENTS Interpreter Development)

set(rpp-pybind_DIRS ${RPP_EXTERNALS_DIR}/pybind)
add_subdirectory(${rpp-pybind_DIRS} ${CMAKE_BINARY_DIR}/externals/pybind)

add_library(rpp-pybind INTERFACE)
target_link_libraries(rpp-pybind INTERFACE pybind11::pybind11)
