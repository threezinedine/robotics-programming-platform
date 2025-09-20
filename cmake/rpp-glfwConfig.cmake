if (rpp-glfw_FOUND)
    return()
endif()

set(rpp-glfw_DIR ${RPP_EXTERNALS_DIR}/glfw)
add_subdirectory(${rpp-glfw_DIR} ${CMAKE_BINARY_DIR}/externals/glfw)

add_library(rpp-glfw INTERFACE)
target_link_libraries(rpp-glfw INTERFACE glfw)

set(rpp-glfw_FOUND TRUE)