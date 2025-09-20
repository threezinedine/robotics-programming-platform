project(RPPPlatforms)
include(RPPUtils_Config)

file(
    GLOB
    PLATFORMS_SRC
    "src/*.cpp"
)

file(
    GLOB
    PLATFORMS_HDR
    "include/platforms/*.h"
)

add_library(${PROJECT_NAME} STATIC ${PLATFORMS_SRC} ${PLATFORMS_HDR})
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

RPPDetectPlatform()

target_compile_definitions(
    ${PROJECT_NAME} 
    PUBLIC 
    ${RPP_PLATFORM_DEFINES}
)