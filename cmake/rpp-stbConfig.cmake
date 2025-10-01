if (rpp-stb_FOUND)
    return()
endif()

set(rpp-stb_DIR "${RPP_EXTERNALS_DIR}/stb")

add_library(rpp-stb INTERFACE)
target_include_directories(rpp-stb INTERFACE "${rpp-stb_DIR}")

set(rpp-stb_FOUND TRUE)