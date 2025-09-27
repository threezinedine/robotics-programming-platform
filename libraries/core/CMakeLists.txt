include(RPPUtils_Externals)
project(RPPCore)

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src/__tests__)

file(
    GLOB
    CORE_SRC
    "src/*.cpp"
    "src/**/*.cpp"
)

file(
    GLOB
    CORE_SRC_TEST
    "${TEST_FOLDER}/*.cpp"
    "${TEST_FOLDER}/*.h"
)

file(
    GLOB
    CORE_HDR
    "include/core/*.h"
    "include/core/**/*.h"
)

# Remove test files from the main source list
foreach(CORE_SRC_TEST_FILE ${CORE_SRC_TEST})
    list(REMOVE_ITEM CORE_SRC ${CORE_SRC_TEST_FILE})
endforeach()

RPPFindPackage(rpp-json "Dependencies")

# -------------- Library target --------------
set(CMAKE_FOLDER "Libraries")
add_library(${PROJECT_NAME} STATIC ${CORE_SRC} ${CORE_HDR})

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    RPPPlatforms
    PRIVATE
    rpp-json
)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(
    ${PROJECT_NAME} 
    PUBLIC 
    ${RPP_BUILD_TYPE_DEFINE}
)

target_precompile_headers(${PROJECT_NAME} PUBLIC "include/core/common.h")

RPPTargetDefines(${PROJECT_NAME})

RPPFindPackage(rpp-googletest "Dependencies")

# -------------- Test target --------------
set(CMAKE_FOLDER "Tests")
set(PROJECT_TEST_NAME ${PROJECT_NAME}_tests)
add_executable(${PROJECT_TEST_NAME} ${CORE_SRC_TEST})
target_link_libraries(${PROJECT_TEST_NAME} PRIVATE rpp-googletest ${PROJECT_NAME})
target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${TEST_FOLDER})
target_precompile_headers(${PROJECT_TEST_NAME} PRIVATE "${TEST_FOLDER}/test_common.h")