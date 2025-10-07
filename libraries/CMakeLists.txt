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

# -------------- Subdirectories --------------
add_subdirectory(platforms)
add_subdirectory(core)
add_subdirectory(modules)
add_subdirectory(applications)
