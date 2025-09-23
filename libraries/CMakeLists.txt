cmake_minimum_required(VERSION 3.20)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(RPPUtils_Config)
include(RPPUtils_Externals)

RPPProjectSetup()
RPPDetectPlatform()
ExternalsSetup()

Project(RPPLibs)

add_subdirectory(platforms)
add_subdirectory(core)
add_subdirectory(modules)

# -------------- Run autogen --------------
set(CMAKE_FOLDER "Custom")
find_package(Python REQUIRED)

set(TIMESTAMP_FILE ${CMAKE_CURRENT_BINARY_DIR}/autogen_timestamp.txt)

# trick for always running autogen when building
add_custom_command(
    OUTPUT ${TIMESTAMP_FILE}
    COMMAND ${CMAKE_COMMAND} -E echo "${CURRENT_TIME}" > ${TIMESTAMP_FILE}
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tmp/binding.cpp
    COMMAND ${Python_EXECUTABLE} ${RPP_BASE_DIR}/config.py -p autogen run
    WORKING_DIRECTORY ${RPP_BASE_DIR}
    DEPENDS ${TIMESTAMP_FILE}
    COMMENT "Running autogen..."
)

add_custom_command(
    OUTPUT ${TIMESTAMP_FILE}
    COMMAND ${CMAKE_COMMAND} -E echo "${CURRENT_TIME}" > ${TIMESTAMP_FILE}
)

add_custom_target(Autogen ALL
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tmp/binding.cpp
)