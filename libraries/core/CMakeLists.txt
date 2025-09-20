project(RPPCore)

file(
    GLOB
    CORE_SRC
    "src/*.cpp"
)

file(
    GLOB
    CORE_HDR
    "include/core/*.h"
)

add_library(${PROJECT_NAME} STATIC ${CORE_SRC} ${CORE_HDR})
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)