if (rpp-googletest_FOUND)
    return()
endif()

set(googletest_DIR ${RPP_EXTERNALS_DIR}/googletest)
add_subdirectory(${googletest_DIR} ${CMAKE_CURRENT_BINARY_DIR}/externals/googletest)
add_library(rpp-googletest INTERFACE)
target_link_libraries(
    rpp-googletest 
    INTERFACE
    gtest 
    gtest_main
    gmock
    gmock_main
)

set(rpp-googletest_FOUND TRUE)