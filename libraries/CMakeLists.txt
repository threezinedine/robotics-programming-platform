cmake_minimum_required(VERSION 3.20)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(RPPUtils_Config)
include(RPPUtils_Externals)

RPPProjectSetup()
RPPDetectPlatform()
ExternalsSetup()

Project(RPPLibs)

# -------------- Run autogen --------------
set(CMAKE_FOLDER "Custom")
find_package(Python REQUIRED)

# trick for always running autogen when building
set(BINDING_FILE ${RPP_BASE_DIR}/libraries/tmp/binding.cpp)
set(WRITER_FILE ${RPP_BASE_DIR}/libraries/tmp/writer_output.cpp)
set(E2E_FILE ${RPP_BASE_DIR}/libraries/tmp/e2e_test_binding.cpp)

add_custom_target(
    Autogen ALL
    COMMAND ${Python_EXECUTABLE} ${RPP_BASE_DIR}/config.py -p autogen run
    WORKING_DIRECTORY ${RPP_BASE_DIR}
    COMMENT "========== Running autogen... =========="
)

include(RPPUtils_Externals)

file(
    GLOB
    LIBRARIES_SRC
    "src/*.cpp"
    "src/**/*.cpp"
    "src/**/**/*.cpp"
)

file(
    GLOB
    LIBRARIES_HDR
    "include/*.h"
    "include/**/*.h"
)

file(
    GLOB
    LIBRARIES_TEST_SRC
    "tests/*.cpp"
    "tests/*.h"
    "tests/**/*.h"
    "tests/**/*.cpp"
)

set(
    GENERATED_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/tmp/writer_output.cpp"
)

set(CMAKE_FOLDER "Libraries")
add_library(
    ${PROJECT_NAME} 
    STATIC 
    ${LIBRARIES_SRC} 
    ${LIBRARIES_HDR}
    ${GENERATED_FILES}
)

RPPFindPackage(rpp-glfw               "Dependencies")
RPPFindPackage(rpp-glm                "Dependencies")
RPPFindPackage(rpp-imgui              "Dependencies")
RPPFindPackage(rpp-imgui-file-browser "Dependencies")
RPPFindPackage(rpp-luna               "Dependencies")
RPPFindPackage(rpp-pybind             "Dependencies")
RPPFindPackage(rpp-stb                "Dependencies")
RPPFindPackage(rpp-json               "Dependencies")

set(
    TARGET_LIBRARIES
    rpp-glfw
    rpp-glm
    rpp-imgui
    rpp-imgui-file-browser
    rpp-luna
    rpp-stb
    rpp-json
)

set(
    TARGET_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC 
    ${TARGET_LIBRARIES}
)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${TARGET_INCLUDE_DIRS}
)

target_compile_definitions(
    ${PROJECT_NAME} 
    PUBLIC 
    ${RPP_BUILD_TYPE_DEFINE}
)

RPPTargetDefines(${PROJECT_NAME})

# -------------- Test Lib ----------------
set(CMAKE_FOLDER "TestLib")
set(PROJECT_NAME_TEST_LIB ${PROJECT_NAME}_test_lib)

add_library(
    ${PROJECT_NAME_TEST_LIB}
    STATIC
    ${LIBRARIES_SRC} 
    ${LIBRARIES_HDR}
    ${GENERATED_FILES}
)

target_link_libraries(
    ${PROJECT_NAME_TEST_LIB} 
    PUBLIC 
    ${TARGET_LIBRARIES}
    rpp-pybind
)

target_include_directories(
    ${PROJECT_NAME_TEST_LIB} 
    PUBLIC 
    ${TARGET_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(
    ${PROJECT_NAME_TEST_LIB} 
    PUBLIC 
    ${RPP_BUILD_TYPE_DEFINE}
    RPP_USE_TEST # Enable test mode for testing purposes only.
)

RPPTargetDefines(${PROJECT_NAME_TEST_LIB})

# -------------- Test target --------------
RPPFindPackage(rpp-googletest "Dependencies")

set(CMAKE_FOLDER "Tests")
set(PROJECT_TEST_NAME ${PROJECT_NAME}_tests)

add_executable(
    ${PROJECT_TEST_NAME} 
    ${LIBRARIES_TEST_SRC}
)

target_include_directories(
    ${PROJECT_TEST_NAME} 
    PRIVATE 
    ${TARGET_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

target_link_libraries(
    ${PROJECT_TEST_NAME} 
    PRIVATE 
    ${PROJECT_NAME_TEST_LIB} 
    rpp-googletest
)

target_compile_definitions(
    ${PROJECT_TEST_NAME}
    PRIVATE
    RPP_LIBRARIES_TEST
)