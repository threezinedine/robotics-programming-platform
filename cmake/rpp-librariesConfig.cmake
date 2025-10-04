if (TARGET rpp-libraries)
    return()
endif()

set(rpp-libraries_DIR ${RPP_BASE_DIR}/libraries)
message("Adding rpp-libraries from: ${rpp-libraries_DIR}")
add_subdirectory(${rpp-libraries_DIR} ${CMAKE_BINARY_DIR}/externals/libraries)