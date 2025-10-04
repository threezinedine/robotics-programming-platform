if (TARGET rpp-glm)
    return()
endif()

set(rpp-glm_DIR "${RPP_EXTERNALS_DIR}/glm")
add_subdirectory(${rpp-glm_DIR} ${CMAKE_BINARY_DIR}/externals/glm)

add_library(rpp-glm INTERFACE)
target_link_libraries(rpp-glm INTERFACE glm::glm)
