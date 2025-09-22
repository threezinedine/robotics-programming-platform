project(RPPPlatforms)

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
    ${RPP_BUILD_TYPE_DEFINE}
)

if (RPP_PLATFORM_NAME STREQUAL "Windows" OR RPP_PLATFORM_NAME STREQUAL "Linux" OR RPP_PLATFORM_NAME STREQUAL "MacOS")
    RPPFindPackage(rpp-glfw "Dependencies")
    target_link_libraries(${PROJECT_NAME} PUBLIC rpp-glfw)
endif()

target_precompile_headers(${PROJECT_NAME} PUBLIC "include/platforms/common.h")