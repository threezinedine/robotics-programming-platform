if (TARGET rpp-libaries)
    return()
endif()

set(rpp-libaries_DIR ${RPP_BASE_DIR}/libraries)
message("Adding rpp-libaries from: ${rpp-libaries_DIR}")
add_subdirectory(${rpp-libaries_DIR} ${CMAKE_BINARY_DIR}/externals/libaries)