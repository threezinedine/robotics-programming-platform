cmake_minimum_required(VERSION 3.20)

project(RPPRuntime)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(RPPUtils_Externals)

ExternalsSetup()

message(STATUS "RPP External Directory: ${RPP_EXTERNALS_DIR}")