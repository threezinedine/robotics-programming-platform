if (rpp-glfw_FOUND)
    return()
endif()

set(glfw_DIR ${RPP_EXTERNALS_DIR}/glfw)
add_subdirectory(${glfw_DIR} ${CMAKE_BINARY_DIR}/externals/glfw)

set(glew_DIR ${RPP_EXTERNALS_DIR}/glew)
set(glew-cmake_BUILD_SHARED FALSE)
add_subdirectory(${glew_DIR} ${CMAKE_BINARY_DIR}/externals/glew)

find_package(OpenGL REQUIRED)

add_library(rpp-glfw INTERFACE)
target_link_libraries(rpp-glfw INTERFACE glfw libglew_static OpenGL::GL)

set(rpp-glfw_FOUND TRUE)