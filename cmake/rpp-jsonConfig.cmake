if (TARGET rpp-json)
    return()
endif()

set(rpp-json_DIR "${RPP_EXTERNALS_DIR}/json")
add_subdirectory(${rpp-json_DIR} ${CMAKE_BINARY_DIR}/externals/json)

add_library(rpp-json INTERFACE)
target_link_libraries(rpp-json INTERFACE nlohmann_json::nlohmann_json)

set(rpp-json_FOUND TRUE)