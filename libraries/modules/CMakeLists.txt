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
RPPFindPackage(rpp-imgui "Dependencies")
RPPFindPackage(rpp-glm "Dependencies")
RPPFindPackage(rpp-luna "Dependencies")
RPPFindPackage(rpp-pybind "Dependencies")
RPPFindPackage(rpp-imgui-notify "Dependencies")

set(LINK_LIBRARIES
    RPPCore
    rpp-json
    rpp-imgui
    rpp-glm
    rpp-luna
    rpp-pybind
    rpp-imgui-notify
)

# -------------- Library target --------------
set(CMAKE_FOLDER "Libraries")
add_library(${PROJECT_NAME} STATIC ${MODULES_SRC} ${MODULES_HDR})

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    ${LINK_LIBRARIES}
)

target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RPP_BASE_DIR}/libraries
)

target_compile_definitions(
    ${PROJECT_NAME} 
    PUBLIC 
    ${RPP_BUILD_TYPE_DEFINE}
)

RPPTargetDefines(${PROJECT_NAME})

# -------------- Testing Libraries --------------
set(CMAKE_FOLDER "TestLib")
set(PROJECT_NAME_TEST_LIB ${PROJECT_NAME}_test_lib)
add_library(${PROJECT_NAME_TEST_LIB} STATIC ${MODULES_SRC} ${MODULES_HDR})

target_link_libraries(
    ${PROJECT_NAME_TEST_LIB}
    PUBLIC
    ${LINK_LIBRARIES}
)

target_include_directories(
    ${PROJECT_NAME_TEST_LIB} 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RPP_BASE_DIR}/libraries
)

target_compile_definitions(
    ${PROJECT_NAME_TEST_LIB}
    PUBLIC 
    ${RPP_BUILD_TYPE_DEFINE}
    RPP_USE_TEST # Enable test mode for testing purposes only.
)

RPPTargetDefines(${PROJECT_NAME_TEST_LIB})

RPPFindPackage(rpp-googletest "Dependencies")

# -------------- Test target --------------
set(CMAKE_FOLDER "Tests")
set(PROJECT_TEST_NAME ${PROJECT_NAME}_tests)
add_executable(${PROJECT_TEST_NAME} ${MODULES_SRC_TEST})
target_link_libraries(${PROJECT_TEST_NAME} PRIVATE rpp-googletest ${PROJECT_NAME_TEST_LIB})
target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${TEST_FOLDER})
target_precompile_headers(${PROJECT_TEST_NAME} PRIVATE "${TEST_FOLDER}/test_common.h")