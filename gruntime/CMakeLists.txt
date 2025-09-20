cmake_minimum_required(VERSION 3.20)

project(RPPGraphicRuntime)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(RPPUtils_Externals)
include(RPPUtils_Config)

RPPProjectSetup()
ExternalsSetup()

# -------------- Graphics Runtime ----------------
file(
    GLOB
    GRUNTIME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

RPPFindPackage(rpp-glfw)
RPPFindPackage(rpp-libraries)

add_executable(${PROJECT_NAME} ${GRUNTIME_SOURCES})
target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    rpp-glfw
    RPPCore
)

# set project as default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})