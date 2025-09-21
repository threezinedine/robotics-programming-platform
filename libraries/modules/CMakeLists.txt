include(RPPUtils_Externals)
project(RPPModules)

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src/__tests__)

file(
    GLOB
    MODULES_SRC
    "src/*.cpp"
    "src/**/*.cpp"
)

file(
    GLOB
    MODULES_SRC_TEST
    "${TEST_FOLDER}/*.cpp"
    "${TEST_FOLDER}/*.h"
)

file(
    GLOB
    MODULES_HDR
    "include/modules/*.h"
    "include/modules/**/*.h"
)

# Remove test files from the main source list
foreach(MODULES_SRC_TEST_FILE ${MODULES_SRC_TEST})
    list(REMOVE_ITEM MODULES_SRC ${MODULES_SRC_TEST_FILE})
endforeach()

RPPFindPackage(rpp-json "Dependencies")

# -------------- Library target --------------
set(CMAKE_FOLDER "Libraries")
add_library(${PROJECT_NAME} STATIC ${MODULES_SRC} ${MODULES_HDR})

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    RPPCore
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

RPPFindPackage(rpp-googletest "Dependencies")

# -------------- Test target --------------
set(CMAKE_FOLDER "Tests")
set(PROJECT_TEST_NAME ${PROJECT_NAME}_tests)
add_executable(${PROJECT_TEST_NAME} ${MODULES_SRC_TEST})
target_link_libraries(${PROJECT_TEST_NAME} PRIVATE rpp-googletest ${PROJECT_NAME})
target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${TEST_FOLDER})
target_precompile_headers(${PROJECT_TEST_NAME} PRIVATE "${TEST_FOLDER}/test_common.h")