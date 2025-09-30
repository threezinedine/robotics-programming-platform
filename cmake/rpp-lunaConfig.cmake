if (rpp-luna_FOUND)
    return()
endif()

set(rpp-luna_DIR "${RPP_EXTERNALS_DIR}/luna")
add_subdirectory(${rpp-luna_DIR} ${CMAKE_BINARY_DIR}/externals/luna)

add_library(rpp-luna INTERFACE)
target_link_libraries(rpp-luna INTERFACE lunasvg plutovg)

set(rpp-luna_FOUND TRUE)