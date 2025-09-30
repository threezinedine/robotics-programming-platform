cmake_minimum_required(VERSION 3.20)
project(RppEditor)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(RPPUtils_Externals)
include(RPPUtils_Config)

RPPProjectSetup()
ExternalsSetup()

# -------------- Editor ----------------
file(
    GLOB
    EDITOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

RPPFindPackage(rpp-libraries "Libraries")
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

# set project as default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})