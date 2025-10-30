cmake_minimum_required(VERSION 3.20)

project(RPPGraphicRuntime)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(RPPUtils_Externals)
include(RPPUtils_Config)

RPPProjectSetup()
ExternalsSetup()

RPPDefineOption(RPP_PYTHON_HOME "C:/Python311")
# RPPDefineOption(RPP_PYTHON_LIBS "C:/Python311/libs")

if (MSVC) # ignore specific warnings on MSVC
    add_compile_options(
        /wd4291
        /wd4003 # TODO: remove later
    )
endif()

# -------------- Graphics Runtime ----------------
file(
    GLOB
    GRUNTIME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

RPPFindPackage(rpp-libraries "Libraries")
RPPFindPackage(rpp-pybind "Dependencies")

add_executable(${PROJECT_NAME} ${GRUNTIME_SOURCES})
target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    RPPApplications
)
target_compile_definitions(
    ${PROJECT_NAME} 
    PRIVATE 
    ${RPP_BUILD_TYPE_DEFINE}
)

if (MSVC)
    # set project as default startup project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

# -------------- Tests ----------------
set(RUNTIME_TEST "${PROJECT_NAME}_test")

add_executable(
    ${RUNTIME_TEST}
    ${GRUNTIME_SOURCES} 
)

target_link_libraries(
    ${RUNTIME_TEST}
    PRIVATE 
    RPPApplications_test_lib
    rpp-pybind-embed
)

target_compile_definitions(
    ${RUNTIME_TEST}
    PRIVATE 
    ${RPP_BUILD_TYPE_DEFINE}
    RPP_USE_TEST
)