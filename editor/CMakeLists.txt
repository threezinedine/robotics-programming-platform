cmake_minimum_required(VERSION 3.20)
project(RppEditor)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(RPPUtils_Externals)
include(RPPUtils_Config)

RPPProjectSetup()
ExternalsSetup()

if (MSVC) # ignore specific warnings on MSVC
    add_compile_options(
        /wd4291
        /wd4003 # TODO: remove later
    )
endif()

# -------------- Editor ----------------
file(
    GLOB
    EDITOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

RPPFindPackage(rpp-libraries "Libraries")
RPPFindPackage(rpp-pybind "Dependencies")

add_executable(${PROJECT_NAME} ${EDITOR_SOURCES})
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
set(EDITOR_TEST "${PROJECT_NAME}_test")
add_executable(
    ${EDITOR_TEST}
    ${EDITOR_SOURCES} 
)

target_link_libraries(
    ${EDITOR_TEST}
    PRIVATE 
    RPPApplications_test_lib
    rpp-pybind-embed
)

target_compile_definitions(
    ${EDITOR_TEST} 
    PRIVATE 
    ${RPP_BUILD_TYPE_DEFINE}
    RPP_USE_TEST
)