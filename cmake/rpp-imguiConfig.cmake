if (rpp-imgui_FOUND)
    return()
endif()

set(rpp-imgui_DIR "${RPP_EXTERNALS_DIR}/imgui")
# add_subdirectory(${rpp-imgui_DIR} ${CMAKE_BINARY_DIR}/externals/rpp-imgui)

RPPFindPackage(rpp-glfw "Dependencies")

file(
    GLOB 
    IMGUI_HEADERS
    "${rpp-imgui_DIR}/*.h"
    "${rpp-imgui_DIR}/backends/imgui_impl_glfw.h"
    "${rpp-imgui_DIR}/backends/imgui_impl_opengl3.h"
)

file(
    GLOB 
    IMGUI_SOURCES
    "${rpp-imgui_DIR}/*.cpp"
    "${rpp-imgui_DIR}/backends/imgui_impl_glfw.cpp"
    "${rpp-imgui_DIR}/backends/imgui_impl_opengl3.cpp"
)

add_library(rpp-imgui STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})
target_include_directories(rpp-imgui PUBLIC ${rpp-imgui_DIR} ${rpp-imgui_DIR}/backends)
target_link_libraries(rpp-imgui PUBLIC rpp-glfw)

set(rpp-imgui_FOUND TRUE)