include(RPPUtils_Externals)
project(RPPApplications)

file(
    GLOB
    APPLICATIONS_SRC
    "src/*.cpp"
    "src/**/*.cpp"
)

file(
    GLOB
    APPLICATIONS_HDR
    "include/applications/*.h"
    "include/applications/**/*.h"
)

file(
    GLOB
    APPLICATIONS_TEST_SRC
    "src/__tests__/*.cpp"
    "src/__tests__/*.h"
    "src/__tests__/**/*.h"
    "src/__tests__/**/*.cpp"
)

set(WRITER_OUTPUT ${RPP_BASE_DIR}/libraries/tmp/writer_output.cpp)
set_source_files_properties(${WRITER_OUTPUT} PROPERTIES GENERATED TRUE)

foreach (APPLICATIONS_TEST_SRC_FILE ${APPLICATIONS_TEST_SRC})
    list(REMOVE_ITEM APPLICATIONS_SRC ${APPLICATIONS_TEST_SRC_FILE})
endforeach()

set(CMAKE_FOLDER "Libraries")
add_library(
    ${PROJECT_NAME} 
    STATIC 
    ${APPLICATIONS_SRC} 
    ${APPLICATIONS_HDR}
    ${WRITER_OUTPUT}
)

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC 
    RPPCore 
    RPPModules
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

RPPTargetDefines(${PROJECT_NAME})


# -------------- Test target --------------
set(CMAKE_FOLDER "Tests")
set(PROJECT_TEST_NAME ${PROJECT_NAME}_tests)
RPPFindPackage(rpp-googletest "Dependencies")
add_executable(${PROJECT_TEST_NAME} ${APPLICATIONS_TEST_SRC})
target_link_libraries(
    ${PROJECT_TEST_NAME} 
    PRIVATE 
    ${PROJECT_NAME} 
    rpp-googletest
)